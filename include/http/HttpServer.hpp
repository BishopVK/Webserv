#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include "ClientConnection.hpp"
#include "Multiplexer.hpp"
#include "Server.hpp"
#include "ServerConnection.hpp"
#include <map>
#include <netdb.h>
#include <vector>

class HttpRequestHandler;

class HttpServer
{
  private:
    std::vector<Server>             _servers;
    std::map<int, ServerConnection> _serverConnections;
    std::map<int, ClientConnection> _clients;
    Multiplexer                     _multiplexer;

    bool handleClientRead(int client_fd, ClientConnection& client, Multiplexer& multiplexer);
    bool handleClientWrite(int client_fd, ClientConnection& client, Multiplexer& multiplexer);

  public:
    HttpServer(const std::vector<Server>& servers);
    HttpServer(const HttpServer& other);
    ~HttpServer();
    HttpServer& operator=(const HttpServer& other);

    void run();
};

#endif
