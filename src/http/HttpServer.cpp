#include "HttpServer.hpp"
#include "ErrorPageGenerator.hpp"
#include "HttpRequest.hpp"
#include "HttpRequestHandler.hpp"
#include "HttpResponse.hpp"
#include "IntValue.hpp"
#include "Logger.hpp"
#include "Multiplexer.hpp"
#include "Port.hpp"
#include "Server.hpp"
#include "SocketUtils.hpp"

#include <cerrno>
#include <cstddef>
#include <cstring>
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
    _serverConnections = other._serverConnections;
    _clients = other._clients;
    _multiplexer = other._multiplexer;
}

HttpServer& HttpServer::operator=(const HttpServer& other)
{
    if (this != &other)
    {
        _servers = other._servers;
        _serverConnections = other._serverConnections;
        _clients = other._clients;
        _multiplexer = other._multiplexer;
    }
    return *this;
}

bool running = true;

void signalHandler(int signum)
{
    (void)signum;
    running = false; // Cambia el estado a falso para salir del bucle principal
}

void HttpServer::run()
{
    // std::vector<int>                server_fds;
    // std::signal(SIGINT, signalHandler);
    // std::signal(SIGTERM, signalHandler);

    //std::cout << GREEN "Webserv initialized" RESET << std::endl;
    std::cout << GREEN
    << " __      __   _                      _      _ _   _      _ _           _ " << std::endl
    << " \\ \\    / /__| |__ ___ ___ _ ___ __ (_)_ _ (_) |_(_)__ _| (_)______ __| |" << std::endl
    << "  \\ \\/\\/ / -_) '_ (_-</ -_) '_\\ V / | | ' \\| |  _| / _` | | |_ / -_) _` |" << std::endl
    << "   \\_/\\_/\\___|_.__/__/\\___|_|  \\_/  |_|_||_|_|\\__|_\\__,_|_|_/__\\___\\__,_|" << std::endl
    << "                                                                         "
    << RESET
    << std::endl;

    for (std::vector<Server>::iterator servers_it = _servers.begin(); servers_it != _servers.end(); ++servers_it)
    {
        std::multimap<std::string, int> listenSet = servers_it->getListenSet();
        for (std::multimap<std::string, int>::iterator listen_it = listenSet.begin(); listen_it != listenSet.end(); ++listen_it)
        {
            std::cout << "listen at: " << YELLOW << listen_it->first << ":" << listen_it->second << RESET << std::endl;
        }
    }

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

            Logger::instance().info("Starting server on " + ip + ":" + port);

            int server_fd = SocketUtils::createServerSocket(ip.c_str(), port.c_str());
            if (server_fd == -1)
            {
                Logger::instance().error("Failed to create server socket on " + ip + ":" + port);
                continue;
            }

            Logger::instance().info("Server socket created successfully on " + ip + ":" + port + " (fd: " + IntValue(server_fd).toString() + ")");

            SocketUtils::setNonBlocking(server_fd);
            SocketUtils::setReuseAddr(server_fd);
            _multiplexer.addFd(server_fd, Multiplexer::READ);
            _serverConnections.insert(std::make_pair(server_fd, ServerConnection(server_fd, &(*it))));
        }
    }

    if (_serverConnections.empty())
    {
        Logger::instance().error("No server sockets available.");
        return;
    }

    //while (true)
    while (running)
    {
        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);
        int ready_count = _multiplexer.poll(1000);

        if (ready_count == -1)
        {
            Logger::instance().error("Poll failed: " + std::string(strerror(errno)));
            break;
        }

        if (ready_count == 0)
            continue;

        for (std::map<int, ServerConnection>::iterator server_it = _serverConnections.begin(); server_it != _serverConnections.end(); ++server_it)
        {
            const ServerConnection& server_conn = server_it->second;
            int                     server_fd = server_conn.getFd();

            if (!_multiplexer.isReadReady(server_fd))
                continue;

            int client_fd = accept(server_fd, NULL, NULL);

            if (client_fd == -1)
            {
                Logger::instance().error("Error accepting connection: " + std::string(strerror(errno)));
                continue;
            }

            SocketUtils::setNonBlocking(client_fd);
            _multiplexer.addFd(client_fd, Multiplexer::READ);
            _clients[client_fd] = ClientConnection();
            _clients[client_fd].setServerConnection(&server_it->second);
            Logger::instance().info("New client connected: " + IntValue(client_fd).toString() + ".");
        }

        std::vector<int> ready_fds = _multiplexer.getReadyFds();
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

            if (_multiplexer.hasError(fd) || _multiplexer.hasHangup(fd))
            {
                Logger::instance().debug("Client disconnected or socket error: " + IntValue(fd).toString() + ".");
                _multiplexer.removeFd(fd);
                _clients.erase(fd);
                SocketUtils::closeSocket(fd);
                continue;
            }

            if (_multiplexer.isReadReady(fd))
                handleClientRead(fd, _clients[fd], _multiplexer);

            if (_multiplexer.isWriteReady(fd))
                handleClientWrite(fd, _clients[fd], _multiplexer);
        }
    }

    Logger::instance().info("Server shutting down, closing " + IntValue(_serverConnections.size()).toString() + " server sockets");

    // Clean up all client connections
    for (std::map<int, ClientConnection>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        Logger::instance().debug("Closing client connection: " + IntValue(it->first).toString());
        std::cout << "Closing client connection: " << YELLOW << IntValue(it->first).toString() << RESET << std::endl;
        SocketUtils::closeSocket(it->first);
    }
    _clients.clear();

    // Clean up server connections
    for (std::map<int, ServerConnection>::iterator it = _serverConnections.begin(); it != _serverConnections.end();)
    {
        SocketUtils::closeSocket(it->second.getFd());
        std::map<int, ServerConnection>::iterator iterator_to_delete = it; // Prevent SegFault
        ++it;
        _serverConnections.erase(iterator_to_delete);
    }

    std::cout << std::endl << RED "Closing webserv..." RESET << std::endl;
}

bool HttpServer::handleClientRead(int client_fd, ClientConnection& client, Multiplexer& multiplexer)
{
    // TODO: Sacar esto a constantes.
    const size_t BUFFER_SIZE = 4096;                  // 4 KB
    const size_t MAX_REQUEST_SIZE = 10 * 1024 * 1024; // 10 MB

    char    buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while (!client.hasCompleteRequest())
    {
        std::memset(buffer, 0, BUFFER_SIZE);
        bytes_read = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);

        if (bytes_read == -1)
            return true;

        if (bytes_read == 0)
        {
            Logger::instance().info("Client " + IntValue(client_fd).toString() + " closed connection");
            multiplexer.removeFd(client_fd);
            SocketUtils::closeSocket(client_fd);
            return false;
        }

        buffer[bytes_read] = '\0';

        if (client.getRequestSize() + bytes_read > MAX_REQUEST_SIZE)
        {
            Logger::instance().error("Request too large from client " + IntValue(client_fd).toString());
            client.setWriteBuffer(ErrorPageGenerator::GenerateErrorResponse(HttpResponse::requestEntityTooLarge("Request too large")).toString());
            client.clearReadBuffer();
            multiplexer.modifyFd(client_fd, Multiplexer::WRITE);
            return true;
        }

        client.appendToReadBuffer(std::string(buffer, bytes_read));

        if (client.hasCompleteRequest())
            break;
    }

    try
    {
        client.setRequestComplete(true);

        HttpRequest* request = client.getHttpRequest();
        if (!request || !request->isValid())
        {
            Logger::instance().error("Invalid HTTP request from client " + IntValue(client_fd).toString());
            client.setWriteBuffer(ErrorPageGenerator::GenerateErrorResponse(HttpResponse::badRequest()).toString());
            client.clearReadBuffer();
            multiplexer.modifyFd(client_fd, Multiplexer::WRITE);
            return true;
        }

        std::string full_request = client.getReadBuffer();
        size_t      body_start = full_request.find("\r\n\r\n");
        if (body_start != std::string::npos)
        {
            body_start += 4; // \r\n\r\n
            std::string body = full_request.substr(body_start);
            request->setBody(body);
        }

        HttpRequestHandler handler;
        HttpResponse       response = handler.handle(*request, client);

        client.setWriteBuffer(response.toString());
        client.clearReadBuffer();

        multiplexer.modifyFd(client_fd, Multiplexer::WRITE);
    }
    catch (const std::exception& e)
    {
        Logger::instance().error("Error processing request: " + std::string(e.what()));
        client.setWriteBuffer(ErrorPageGenerator::GenerateErrorResponse(HttpResponse::internalServerError()).toString());
        client.clearReadBuffer();
        multiplexer.modifyFd(client_fd, Multiplexer::WRITE);
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
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return true;

        Logger::instance().error("Error writing to client: " + std::string(strerror(errno)));
        multiplexer.removeFd(client_fd);
        SocketUtils::closeSocket(client_fd);
        return false;
    }

    client.eraseFromWriteBuffer(0, bytes_sent);

    if (client.getWriteBuffer().empty())
    {
        Logger::instance().debug("Response sent completely to client " + IntValue(client_fd).toString());
        multiplexer.removeFd(client_fd);
        SocketUtils::closeSocket(client_fd);
        return false;
    }

    return true;
}
