#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include "Server.hpp"
#include "Multiplexer.hpp"
#include <netdb.h>
#include <string>
#include <vector>

class HttpRequestHandler;

class HttpServer
{
  public:
    struct ClientConnection
    {
        std::string read_buffer;
        std::string write_buffer;
        bool        request_complete;
        bool        response_sent;

        ClientConnection() : request_complete(false), response_sent(false)
        {
        }
    };

  private:
    // std::vector<std::string> _ports;
    std::vector<Server>      _servers;

    bool handleClientRead(int client_fd, ClientConnection& client, Multiplexer& multiplexer);
    bool handleClientWrite(int client_fd, ClientConnection& client, Multiplexer& multiplexer);

  public:
    // HttpServer(const std::string& port);
    // HttpServer(const std::vector<std::string>& ports);
    HttpServer(const std::vector<Server>& servers);
    HttpServer(const HttpServer& other);
    ~HttpServer();
    HttpServer& operator=(const HttpServer& other);

    void run();
};

#endif
