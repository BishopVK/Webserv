#ifndef HTTP_REQUEST_HANDLER_HPP
#define HTTP_REQUEST_HANDLER_HPP

#include "ClientConnection.hpp"
#include <string>
#include <vector>

class HttpRequest;
class HttpResponse;
class Server;
class Location;

class HttpRequestHandler
{
  private:
    HttpResponse handleWithLocation(const Location& location, const std::string& requestPath, const Server* server) const;
    HttpResponse handleWithServerDefaults(const std::string& requestPath, const Server* server) const;
    HttpResponse handleResource(const std::string& fullPath, const std::string& requestPath, const Location* location, const Server* server) const;
    HttpResponse handleDirectory(const std::string& fullPath, const std::string& requestPath, const Location* location, const Server* server) const;
    HttpResponse handleFile(const std::string& fullPath, const std::string& requestPath, const Location* location, const Server* server) const;
    HttpResponse createErrorResponse(int statusCode, const Location* location, const Server* server) const;

    std::vector<std::string> getIndexFiles(const Location* location, const Server* server) const;
    std::string              getErrorPage(int statusCode, const Location* location, const Server* server) const;

  public:
    HttpRequestHandler();
    HttpRequestHandler(const HttpRequestHandler& other);
    HttpRequestHandler& operator=(const HttpRequestHandler& other);
    ~HttpRequestHandler();

    HttpResponse handle(const HttpRequest& request, const ClientConnection& client) const;
};

#endif
