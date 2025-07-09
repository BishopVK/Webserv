#include "ServerConnection.hpp"

ServerConnection::ServerConnection() : _fd(-1)
{
}

ServerConnection::ServerConnection(int fd) : _fd(fd)
{
}

ServerConnection::ServerConnection(const ServerConnection& other) : _fd(other._fd)
{
}

ServerConnection& ServerConnection::operator=(const ServerConnection& other)
{
    if (this != &other)
    {
        _fd = other._fd;
    }
    return *this;
}

ServerConnection::~ServerConnection()
{
}

int ServerConnection::getFd() const
{
    return _fd;
}

void ServerConnection::setFd(int fd)
{
    _fd = fd;
}

bool ServerConnection::isValid() const
{
    return _fd >= 0;
}
