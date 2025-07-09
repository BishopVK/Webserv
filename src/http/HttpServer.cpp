#include "HttpServer.hpp"
#include "HttpRequest.hpp"
#include "HttpRequestHandler.hpp"
#include "HttpResponse.hpp"
#include "Logger.hpp"
#include "Multiplexer.hpp"
#include "Port.hpp"
#include "Server.hpp"
#include "SocketUtils.hpp"
#include <errno.h>
#include <exception>
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
        std::vector<int> ports = it->getPorts();

        if (ports.empty())
        {
            Logger::instance().error("No existen puertos para el servidor.");
            continue;
        }

        for (std::vector<int>::iterator port_it = ports.begin(); port_it != ports.end(); ++port_it)
        {
            // TODO: Esto puede lanzar una excepción si el puerto es inválido!!
            std::string port = Port(*port_it).toString();
            if (port.empty())
            {
                Logger::instance().error("Puerto inválido: " + IntValue(*port_it).toString() + ".");
                continue;
            }

            Logger::instance().info("Iniciando servidor en el puerto: " + port + ".");

            int server_fd = SocketUtils::createServerSocket(port.c_str());
            if (server_fd == -1)
            {
                Logger::instance().error("No se pudo crear el socket del servidor en el puerto: " + port + ".");
                continue;
            }

            SocketUtils::setNonBlocking(server_fd);
            SocketUtils::setReuseAddr(server_fd);
            multiplexer.addFd(server_fd, Multiplexer::READ);
            // TODO: Esto deberia de estar en el servidor junto con el puerto
            // server_fds.push_back(server_fd);
            _serverConnections[port] = ServerConnection(server_fd);
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

        // for (size_t i = 0; i < server_fds.size(); ++i)
        // {
        //     int server_fd = server_fds[i];
        //     if (multiplexer.isReadReady(server_fd))
        //     {
        //         int client_fd = accept(server_fd, NULL, NULL);
        //         if (client_fd != -1)
        //         {
        //             SocketUtils::setNonBlocking(client_fd);
        //             multiplexer.addFd(client_fd, Multiplexer::READ);
        //             clients[client_fd] = ClientConnection();
        //             Logger::instance().info("Nuevo cliente conectado: " + IntValue(client_fd).toString() + ".");
        //         }
        //     }
        // }

        for (std::map<std::string, ServerConnection>::iterator server_it = _serverConnections.begin();
             server_it != _serverConnections.end(); ++server_it)
        {
            const ServerConnection& server_conn = server_it->second;
            int                     server_fd = server_conn.fd;

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
            Logger::instance().info("Nuevo cliente conectado: " + IntValue(client_fd).toString() + ".");
        }

        std::vector<int> ready_fds = multiplexer.getReadyFds();
        for (std::vector<int>::iterator it = ready_fds.begin(); it != ready_fds.end(); ++it)
        {
            int  fd = *it;
            bool is_server_fd = false;

            for (std::map<std::string, ServerConnection>::iterator server_it = _serverConnections.begin();
                 server_it != _serverConnections.end(); ++server_it)
            {
                if (server_it->second.fd == fd)
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

    for (std::map<std::string, ServerConnection>::iterator it = _serverConnections.begin(); it != _serverConnections.end(); ++it)
    {
        SocketUtils::closeSocket(it->second.fd);
        _serverConnections.erase(it);
    }
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
        try
        {
            client.request_complete = true;

            HttpRequest        request(client.read_buffer.c_str());
            HttpRequestHandler handler;
            HttpResponse       response = handler.handle(request);

            client.write_buffer = response.toString();

            multiplexer.modifyFd(client_fd, Multiplexer::WRITE);
        }
        catch (const std::exception& e)
        {
            client.write_buffer = HttpResponse::internalServerError(e.what()).toString();

            multiplexer.modifyFd(client_fd, Multiplexer::WRITE);
        }
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
