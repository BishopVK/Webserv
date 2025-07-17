#include "SocketUtils.hpp"
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

bool SocketUtils::setNonBlocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
        return false;

    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
        return false;

    return true;
}

// Se establece SO_REUSEADDR para permitir que el socket se pueda reutilizar
bool SocketUtils::setReuseAddr(int fd)
{
    int yes = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        return false;
    return true;
}

// int SocketUtils::createServerSocket(const char* port)
int SocketUtils::createServerSocket(const char* ip, const char* port)
{
    struct addrinfo hints, *res, *p;
    int             server_fd;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int rv = getaddrinfo(ip, port, &hints, &res);
    if (rv != 0)
    {
        std::cerr << "getaddrinfo: " << gai_strerror(rv) << std::endl;
        return -1;
    }

    for (p = res; p != NULL; p = p->ai_next)
    {
        server_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (server_fd == -1)
            continue;

        if (!setReuseAddr(server_fd))
        {
            close(server_fd);
            continue;
        }

        if (bind(server_fd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(server_fd);
            continue;
        }

        break;
    }

    freeaddrinfo(res);

    if (p == NULL)
        return -1;

    if (listen(server_fd, 10) == -1)
    {
        close(server_fd);
        return -1;
    }

    if (!setNonBlocking(server_fd))
    {
        close(server_fd);
        return -1;
    }

    return server_fd;
}

void SocketUtils::closeSocket(int fd)
{
    if (fd >= 0)
        close(fd);
}
