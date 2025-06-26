#pragma once

#include <map>
#include <poll.h>
#include <vector>

class Multiplexer
{
  public:
    enum EventType
    {
        READ = POLLIN,
        WRITE = POLLOUT,
        ERROR = POLLERR | POLLHUP | POLLNVAL
    };

  private:
    std::vector<struct pollfd> _poll_fds;
    std::map<int, std::size_t> _fd_to_index;

    bool isValidFd(int fd) const;

  public:
    Multiplexer();
    ~Multiplexer();

    Multiplexer(const Multiplexer& other);
    Multiplexer& operator=(const Multiplexer& other);

    bool addFd(int fd, short events);
    bool removeFd(int fd);
    bool modifyFd(int fd, short events);

    int poll(int timeout_ms = -1);

    bool isReadReady(int fd) const;
    bool isWriteReady(int fd) const;
    bool hasError(int fd) const;
    bool hasHangup(int fd) const;

    std::vector<int> getReadyFds() const;
};
