#ifndef HTTP_REQUEST_HANDLER_HPP
#define HTTP_REQUEST_HANDLER_HPP

#include "../controllers/ControllerRegistry.hpp"

class HttpRequest;
class HttpResponse;
class BadRequestHttpResponse;

class HttpRequestHandler
{
  private:
    ControllerRegistry _registry;

  public:
    HttpRequestHandler();
    HttpRequestHandler(const HttpRequestHandler& other);
    HttpRequestHandler& operator=(const HttpRequestHandler& other);
    ~HttpRequestHandler();

    HttpResponse handle(const HttpRequest& request);
};

#endif
