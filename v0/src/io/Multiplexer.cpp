#include "Multiplexer.hpp"
#include <cstring>
#include <errno.h>
#include <iostream>

Multiplexer::Multiplexer()
{
}

Multiplexer::~Multiplexer()
{
    // Los file descriptors se cierran en el código que los creó
    // Este destructor solo limpia las estructuras internas
}

Multiplexer::Multiplexer(const Multiplexer& other)
{
    (void)other;
    // Deliberadamente no implementado - no debe copiarse
}

Multiplexer& Multiplexer::operator=(const Multiplexer& other)
{
    (void)other;
    return *this;
    // Deliberadamente no implementado - no debe copiarse
}

bool Multiplexer::addFd(int fd, short events)
{
    if (!isValidFd(fd))
        return false;

    // Verificar si el fd ya existe
    if (_fd_to_index.find(fd) != _fd_to_index.end())
        return false;

    struct pollfd new_pollfd;
    new_pollfd.fd = fd;
    new_pollfd.events = events;
    new_pollfd.revents = 0;

    _poll_fds.push_back(new_pollfd);
    _fd_to_index[fd] = _poll_fds.size() - 1;

    return true;
}

bool Multiplexer::removeFd(int fd)
{
    std::map<int, std::size_t>::iterator it = _fd_to_index.find(fd);
    if (it == _fd_to_index.end())
        return false;

    std::size_t index = it->second;

    // Mover el último elemento a la posición del elemento a eliminar
    if (index != _poll_fds.size() - 1)
    {
        _poll_fds[index] = _poll_fds.back();
        // Actualizar el índice del elemento movido
        _fd_to_index[_poll_fds[index].fd] = index;
    }

    _poll_fds.pop_back();
    _fd_to_index.erase(it);

    return true;
}

bool Multiplexer::modifyFd(int fd, short events)
{
    std::map<int, std::size_t>::iterator it = _fd_to_index.find(fd);
    if (it == _fd_to_index.end())
        return false;

    _poll_fds[it->second].events = events;
    _poll_fds[it->second].revents = 0;
    return true;
}

int Multiplexer::poll(int timeout_ms)
{
    if (_poll_fds.empty())
        return 0;

    int result = ::poll(&_poll_fds[0], _poll_fds.size(), timeout_ms);

    if (result == -1)
    {
        std::cerr << "Poll error: " << strerror(errno) << std::endl;
        return -1;
    }

    return result;
}

bool Multiplexer::isReadReady(int fd) const
{
    std::map<int, std::size_t>::const_iterator it = _fd_to_index.find(fd);
    if (it == _fd_to_index.end())
        return false;

    return (_poll_fds[it->second].revents & POLLIN) != 0;
}

bool Multiplexer::isWriteReady(int fd) const
{
    std::map<int, std::size_t>::const_iterator it = _fd_to_index.find(fd);
    if (it == _fd_to_index.end())
        return false;

    return (_poll_fds[it->second].revents & POLLOUT) != 0;
}

bool Multiplexer::hasError(int fd) const
{
    std::map<int, std::size_t>::const_iterator it = _fd_to_index.find(fd);
    if (it == _fd_to_index.end())
        return false;

    return (_poll_fds[it->second].revents & (POLLERR | POLLNVAL)) != 0;
}

bool Multiplexer::hasHangup(int fd) const
{
    std::map<int, std::size_t>::const_iterator it = _fd_to_index.find(fd);
    if (it == _fd_to_index.end())
        return false;

    return (_poll_fds[it->second].revents & POLLHUP) != 0;
}

std::vector<int> Multiplexer::getReadyFds() const
{
    std::vector<int> ready_fds;
    for (size_t i = 0; i < _poll_fds.size(); ++i)
    {
        if (_poll_fds[i].revents != 0)
            ready_fds.push_back(_poll_fds[i].fd);
    }
    return ready_fds;
}

bool Multiplexer::isValidFd(int fd) const
{
    return fd >= 0;
}
