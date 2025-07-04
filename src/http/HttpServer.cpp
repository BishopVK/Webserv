#include "HttpServer.hpp"
#include "Server.hpp"
#include "Port.hpp"
#include "HttpRequest.hpp"
#include "HttpRequestHandler.hpp"
#include "HttpResponse.hpp"
#include "Multiplexer.hpp"
#include "SocketUtils.hpp"
#include <errno.h>
#include <iostream>
#include <map>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

HttpServer::HttpServer(const std::vector<Server>& servers)
{
    _servers = servers;
}

HttpServer::~HttpServer()
{
}

HttpServer::HttpServer(const HttpServer& other)
{
    _servers = other._servers;
}

HttpServer& HttpServer::operator=(const HttpServer& other)
{
    if (this != &other)
    {
        _servers = other._servers;
    }
    return *this;
}

void HttpServer::run()
{
    Multiplexer                     multiplexer;
    std::map<int, ClientConnection> clients;
    std::vector<int>                server_fds;

    for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); ++it)
    {
        std::vector<int> ports = it->getPorts();

        if (ports.empty())
        {
            std::cerr << "No ports defined for server." << std::endl;
            continue;
        }

        for (std::vector<int>::iterator port_it = ports.begin(); port_it != ports.end(); ++port_it)
        {
            // TODO: Esto puede lanzar una excepción si el puerto es inválido!!
            std::string port = Port(*port_it).toString();
            if (port.empty())
            {
                std::cerr << "Invalid port: " << *port_it << "." << std::endl;
                continue;
            }

            std::cout << "Starting server on port: " << port << std::endl;

            int server_fd = SocketUtils::createServerSocket(port.c_str());
            if (server_fd == -1)
            {
                std::cerr << "Failed to create server socket on port " << port << std::endl;
                continue;
            }

            SocketUtils::setNonBlocking(server_fd);
            SocketUtils::setReuseAddr(server_fd);
            multiplexer.addFd(server_fd, Multiplexer::READ);
            // TODO: Esto deberia de estar en el servidor junto con el puerto
            server_fds.push_back(server_fd);
        }
    }

    if (server_fds.empty())
    {
        std::cerr << "No server sockets available. Exiting." << std::endl;
        return;
    }

    while (true)
    {
        int ready_count = multiplexer.poll(1000);

        if (ready_count == -1)
        {
            std::cerr << "Poll failed" << std::endl;
            break;
        }

        if (ready_count == 0)
            continue;

        for (size_t i = 0; i < server_fds.size(); ++i)
        {
            int server_fd = server_fds[i];
            if (multiplexer.isReadReady(server_fd))
            {
                int client_fd = accept(server_fd, NULL, NULL);
                if (client_fd != -1)
                {
                    SocketUtils::setNonBlocking(client_fd);
                    multiplexer.addFd(client_fd, Multiplexer::READ);
                    clients[client_fd] = ClientConnection();
                    std::cout << "New client connected: " << client_fd << std::endl;
                }
            }
        }

        std::vector<int> ready_fds = multiplexer.getReadyFds();
        for (std::vector<int>::iterator it = ready_fds.begin(); it != ready_fds.end(); ++it)
        {
            int  fd = *it;
            bool is_server_fd = false;
            for (size_t j = 0; j < server_fds.size(); ++j)
            {
                if (server_fds[j] == fd)
                {
                    is_server_fd = true;
                    break;
                }
            }
            if (is_server_fd)
                continue;

            if (multiplexer.hasError(fd) || multiplexer.hasHangup(fd))
            {
                std::cout << "Client disconnected: " << fd << std::endl;
                multiplexer.removeFd(fd);
                clients.erase(fd);
                SocketUtils::closeSocket(fd);
                continue;
            }

            if (multiplexer.isReadReady(fd))
                handleClientRead(fd, clients[fd], multiplexer);

            if (multiplexer.isWriteReady(fd))
                handleClientWrite(fd, clients[fd], multiplexer);
        }
    }

    for (size_t i = 0; i < server_fds.size(); ++i)
        SocketUtils::closeSocket(server_fds[i]);
}

bool HttpServer::handleClientRead(int client_fd, ClientConnection& client, Multiplexer& multiplexer)
{
    char    buffer[4096];
    ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes_read <= 0)
    {
        if (bytes_read == 0 || (bytes_read == -1 && errno != EAGAIN && errno != EWOULDBLOCK))
        {
            multiplexer.removeFd(client_fd);
            SocketUtils::closeSocket(client_fd);
            return false;
        }
        return true;
    }

    buffer[bytes_read] = '\0';
    client.read_buffer += buffer;

    if (client.read_buffer.find("\r\n\r\n") != std::string::npos)
    {
        client.request_complete = true;

        HttpRequest        request(client.read_buffer.c_str());
        HttpRequestHandler handler;
        HttpResponse       response = handler.handle(request);

        client.write_buffer = response.toString();

        multiplexer.modifyFd(client_fd, Multiplexer::WRITE);
    }

    return true;
}

bool HttpServer::handleClientWrite(int client_fd, ClientConnection& client, Multiplexer& multiplexer)
{
    if (client.write_buffer.empty())
        return true;

    ssize_t bytes_sent = send(client_fd, client.write_buffer.c_str(), client.write_buffer.size(), 0);

    if (bytes_sent == -1)
    {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
        {
            multiplexer.removeFd(client_fd);
            SocketUtils::closeSocket(client_fd);
            return false;
        }
        return true;
    }

    client.write_buffer.erase(0, bytes_sent);

    if (client.write_buffer.empty())
    {
        multiplexer.removeFd(client_fd);
        SocketUtils::closeSocket(client_fd);
        return false;
    }

    return true;
}
