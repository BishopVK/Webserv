#include "HttpServer.hpp"
#include "../http/HttpRequestHandler.hpp"
#include "../http/HttpRequest.hpp"
#include "../http/HttpResponse.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

HttpServer::HttpServer(const std::string& port) : _port(port), _server_fd(-1), _res(NULL)
{
}

HttpServer::~HttpServer()
{
    cleanup();
}

HttpServer::HttpServer(const HttpServer& other) : _port(other._port), _server_fd(-1), _res(NULL)
{
}

HttpServer& HttpServer::operator=(const HttpServer& other)
{
    if (this != &other)
    {
        cleanup();
        _port = other._port;
        _server_fd = -1;
        _res = NULL;
    }
    return *this;
}

bool HttpServer::setupSocket()
{
    struct addrinfo hints;
    int             yes = 1;
    int             rv;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, _port.c_str(), &hints, &_res)) != 0)
    {
        std::cerr << "getaddrinfo: " << gai_strerror(rv) << std::endl;
        return false;
    }

    struct addrinfo* p;
    for (p = _res; p != NULL; p = p->ai_next)
    {
        _server_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (_server_fd == -1)
            continue;

        if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            std::cerr << "Failed to set socket options for port " << _port << std::endl;
            close(_server_fd);
            continue;
        }

        if (bind(_server_fd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(_server_fd);
            continue;
        }

        break;
    }

    if (p == NULL)
    {
        std::cerr << "Failed to bind to port " << _port << std::endl;
        freeaddrinfo(_res);
        _res = NULL;
        return false;
    }

    if (listen(_server_fd, 10) == -1)
    {
        std::cerr << "Failed to listen on port " << _port << std::endl;
        close(_server_fd);
        _server_fd = -1;
        freeaddrinfo(_res);
        _res = NULL;
        return false;
    }

    std::cout << "Listening on port " << _port << "..." << std::endl;
    return true;
}

void HttpServer::run()
{
    if (!setupSocket())
        return;

    while (1)
    {
        handleClient();
    }
}

void HttpServer::handleClient()
{
    int client_fd = accept(_server_fd, NULL, NULL);
    if (client_fd == -1)
    {
        perror("accept");
        return;
    }

    char buf[1024];
    int  n = read(client_fd, buf, sizeof(buf) - 1);
    if (n > 0)
    {
        buf[n] = '\0';

        HttpRequest        request(buf);
        HttpRequestHandler handler;
        HttpResponse       response = handler.handle(request);

        std::string respStr = response.toString();
        write(client_fd, respStr.c_str(), respStr.size());
    }
    close(client_fd);
}

void HttpServer::cleanup()
{
    if (_server_fd != -1)
    {
        close(_server_fd);
        _server_fd = -1;
    }
    if (_res)
    {
        freeaddrinfo(_res);
        _res = NULL;
    }
}
