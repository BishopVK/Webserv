#include "HttpServer.hpp"
#include "HttpRequest.hpp"
#include "HttpRequestHandler.hpp"
#include "HttpResponse.hpp"
#include "IntValue.hpp"
#include "Logger.hpp"
#include "Multiplexer.hpp"
#include "Port.hpp"
#include "Server.hpp"
#include "SocketUtils.hpp"

#include <errno.h>
#include <exception>
#include <iostream>
#include <map>
#include <string>
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
    // std::vector<int>                server_fds;

    for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); ++it)
    {
        std::multimap<std::string, int> listen_set = it->getListenSet();

        if (listen_set.empty())
        {
            std::cerr << "No listen entries defined for server." << std::endl;
            continue;
        }

        for (std::multimap<std::string, int>::iterator listen_it = listen_set.begin(); listen_it != listen_set.end(); ++listen_it)
        {
            const std::string& ip = listen_it->first;
            int                port_num = listen_it->second;

            std::string port = Port(port_num).toString();
            if (port.empty())
            {
                std::cerr << "Invalid port: " << port_num << "." << std::endl;
                continue;
            }

            std::cout << "Starting server on " << ip << ":" << port << std::endl;

            int server_fd = SocketUtils::createServerSocket(ip.c_str(), port.c_str());
            if (server_fd == -1)
            {
                std::cerr << "Failed to create server socket on " << ip << ":" << port << std::endl;
                continue;
            }

            SocketUtils::setNonBlocking(server_fd);
            SocketUtils::setReuseAddr(server_fd);
            multiplexer.addFd(server_fd, Multiplexer::READ);
            _serverConnections.insert(std::make_pair(server_fd, ServerConnection(server_fd, &(*it))));
        }
    }

    if (_serverConnections.empty())
    {
        Logger::instance().error("No hay sockets de servidor disponibles.");
        return;
    }

    while (true)
    {
        int ready_count = multiplexer.poll(1000);

        if (ready_count == -1)
        {
            Logger::instance().error("Poll fallido: " + std::string(strerror(errno)));
            break;
        }

        if (ready_count == 0)
            continue;

        for (std::map<int, ServerConnection>::iterator server_it = _serverConnections.begin(); server_it != _serverConnections.end(); ++server_it)
        {
            const ServerConnection& server_conn = server_it->second;
            int                     server_fd = server_conn.getFd();

            if (!multiplexer.isReadReady(server_fd))
                continue;

            int client_fd = accept(server_fd, NULL, NULL);

            if (client_fd == -1)
            {
                Logger::instance().error("Error al aceptar conexión: " + std::string(strerror(errno)));
                continue;
            }

            SocketUtils::setNonBlocking(client_fd);
            multiplexer.addFd(client_fd, Multiplexer::READ);
            clients[client_fd] = ClientConnection();
            clients[client_fd].setServerConnection(&server_it->second);
            Logger::instance().info("Nuevo cliente conectado: " + IntValue(client_fd).toString() + ".");
        }

        std::vector<int> ready_fds = multiplexer.getReadyFds();
        for (std::vector<int>::iterator it = ready_fds.begin(); it != ready_fds.end(); ++it)
        {
            int  fd = *it;
            bool is_server_fd = false;

            for (std::map<int, ServerConnection>::iterator server_it = _serverConnections.begin(); server_it != _serverConnections.end(); ++server_it)
            {
                if (server_it->second.getFd() == fd)
                {
                    is_server_fd = true;
                    break;
                }
            }
            if (is_server_fd)
                continue;

            if (multiplexer.hasError(fd) || multiplexer.hasHangup(fd))
            {
                Logger::instance().debug("Cliente desconectado o error en el socket: " + IntValue(fd).toString() + ".");
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

    for (std::map<int, ServerConnection>::iterator it = _serverConnections.begin(); it != _serverConnections.end(); ++it)
    {
        SocketUtils::closeSocket(it->second.getFd());
        _serverConnections.erase(it);
    }
}

bool HttpServer::handleClientRead(int client_fd, ClientConnection& client, Multiplexer& multiplexer)
{
    char    buffer[100000];
    ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0)
        return true;

    buffer[bytes_read] = '\0';
    client.appendToReadBuffer(buffer);

    while (!client.hasCompleteRequest())
    {
        char    additionalBuffer[100000];
        ssize_t additionalBytes = recv(client_fd, additionalBuffer, sizeof(additionalBuffer) - 1, 0);

        additionalBuffer[additionalBytes] = '\0';
        client.appendToReadBuffer(additionalBuffer);
        bytes_read += additionalBytes;
    }

    if (client.hasCompleteRequest())
    {
        try
        {
            client.setRequestComplete(true);

            HttpRequest request(client.getReadBuffer().c_str());
            if (!request.isValid())
            {
                Logger::instance().error("Invalid HTTP request from client " + IntValue(client_fd).toString());
                client.setWriteBuffer(HttpResponse::badRequest("Invalid HTTP request").toString());
                client.clearReadBuffer();
                multiplexer.modifyFd(client_fd, Multiplexer::WRITE);
                return true;
            }

            HttpRequestHandler handler;
            HttpResponse       response = handler.handle(request, client);

            client.setWriteBuffer(response.toString());
            client.clearReadBuffer();

            multiplexer.modifyFd(client_fd, Multiplexer::WRITE);
        }
        catch (const std::exception& e)
        {
            Logger::instance().error("Error processing request: " + std::string(e.what()));
            client.setWriteBuffer(HttpResponse::internalServerError(e.what()).toString());
            client.clearReadBuffer();

            multiplexer.modifyFd(client_fd, Multiplexer::WRITE);
        }
    }
    else
    {
        Logger::instance().debug("Incomplete request, waiting for more data from client " + IntValue(client_fd).toString());
    }

    return true;
}

bool HttpServer::handleClientWrite(int client_fd, ClientConnection& client, Multiplexer& multiplexer)
{
    if (client.getWriteBuffer().empty())
        return true;

    ssize_t bytes_sent = send(client_fd, client.getWriteBuffer().c_str(), client.getWriteBuffer().size(), 0);

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

    client.eraseFromWriteBuffer(0, bytes_sent);

    if (client.getWriteBuffer().empty())
    {
        multiplexer.removeFd(client_fd);
        SocketUtils::closeSocket(client_fd);
        return false;
    }

    return true;
}
