#ifndef HTTP_POST_REQUEST_HANDLER_HPP
#define HTTP_POST_REQUEST_HANDLER_HPP

#include "AbstractHttpMethodHandler.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Location.hpp"
#include "Server.hpp"

class HttpPostRequestHandler : public AbstractHttpMethodHandler
{
  private:
    virtual HttpResponse handleMethod(const HttpRequest& request, const Location* location, const Server* server) const;

    HttpResponse handleCgi(const HttpRequest& request, const Location* location, const Server* server) const;

  public:
    HttpPostRequestHandler();
    HttpPostRequestHandler(const HttpPostRequestHandler& other);
    HttpPostRequestHandler& operator=(const HttpPostRequestHandler& other);
    virtual ~HttpPostRequestHandler();
};

#endif
