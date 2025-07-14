#ifndef HTTP_REQUEST_HANDLER_HPP
#define HTTP_REQUEST_HANDLER_HPP

#include "ClientConnection.hpp"
#include "HttpDeleteRequestHandler.hpp"
#include "HttpGetRequestHandler.hpp"
#include "HttpPostRequestHandler.hpp"

class HttpRequest;
class HttpResponse;
class Server;
class Location;

class HttpRequestHandler
{
  private:
    HttpGetRequestHandler    _getHandler;
    HttpPostRequestHandler   _postHandler;
    HttpDeleteRequestHandler _deleteHandler;

  public:
    HttpRequestHandler();
    HttpRequestHandler(const HttpRequestHandler& other);
    HttpRequestHandler& operator=(const HttpRequestHandler& other);
    ~HttpRequestHandler();

    HttpResponse handle(const HttpRequest& request, const ClientConnection& client) const;
};

#endif
