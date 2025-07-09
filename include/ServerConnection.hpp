#ifndef SERVERCONNECTION_HPP
#define SERVERCONNECTION_HPP

#include "Server.hpp"

class ServerConnection
{
  private:
    int _fd;
    Server* _server;

  public:
    ServerConnection();
    ServerConnection(int fd, Server* server);
    ServerConnection(const ServerConnection& other);
    ServerConnection& operator=(const ServerConnection& other);
    ~ServerConnection();

    // Getters
    int getFd() const;
    Server* getServer() const;
    const Server& getServerRef() const;
    
    // Setters
    void setFd(int fd);
    void setServer(Server* server);
    
    // Utility methods
    bool isValid() const;
    bool hasServer() const;
};

#endif
