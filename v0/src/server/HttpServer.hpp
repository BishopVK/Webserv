#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <netdb.h>
#include <string>

class HttpRequestHandler;

class HttpServer
{
  private:
    std::string      _port;
    int              _server_fd;
    struct addrinfo* _res;

    bool setupSocket();
    void handleClient();
    void cleanup();

  public:
    HttpServer(const std::string& port);
    HttpServer(const HttpServer& other);
    ~HttpServer();
    HttpServer& operator=(const HttpServer& other);

    void run();
};

#endif
