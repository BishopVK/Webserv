#ifndef SERVERCONNECTION_HPP
#define SERVERCONNECTION_HPP

class ServerConnection
{
  private:
    int _fd;
    // Server* _server;

  public:
    ServerConnection();
    ServerConnection(int fd);
    ServerConnection(const ServerConnection& other);
    ServerConnection& operator=(const ServerConnection& other);
    ~ServerConnection();

    // Getters
    int getFd() const;

    // Setters
    void setFd(int fd);

    // Utility methods
    bool isValid() const;
};

#endif
