#include "ServerConnection.hpp"

ServerConnection::ServerConnection() : _fd(-1), _server(NULL)
{
}

ServerConnection::ServerConnection(int fd, Server* server) : _fd(fd), _server(server)
{
}

ServerConnection::ServerConnection(const ServerConnection& other) : _fd(other._fd), _server(other._server)
{
}

ServerConnection& ServerConnection::operator=(const ServerConnection& other)
{
    if (this != &other)
    {
        _fd = other._fd;
        _server = other._server;
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

Server* ServerConnection::getServer() const
{
    return _server;
}

const Server& ServerConnection::getServerRef() const
{
    return *_server;
}

// Setters
void ServerConnection::setFd(int fd)
{
    _fd = fd;
}

void ServerConnection::setServer(Server* server)
{
    _server = server;
}

bool ServerConnection::isValid() const
{
    return _fd >= 0 && _server != NULL;
}

bool ServerConnection::hasServer() const
{
    return _server != NULL;
}
