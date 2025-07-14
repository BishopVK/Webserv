#ifndef HTTP_GET_REQUEST_HANDLER_HPP
#define HTTP_GET_REQUEST_HANDLER_HPP

#include "AbstractHttpMethodHandler.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include <string>

class HttpGetRequestHandler : public AbstractHttpMethodHandler
{
  private:
    virtual HttpResponse handleMethod(const HttpRequest& request, const Location* location, const Server* server) const;

    HttpResponse handleWithLocation(const Location& location, const std::string& requestPath, const Server* server) const;
    HttpResponse handleWithServerDefaults(const std::string& requestPath, const Server* server) const;
    HttpResponse handleResource(const std::string& fullPath, const std::string& requestPath, const Location* location, const Server* server) const;
    HttpResponse handleDirectory(const std::string& fullPath, const std::string& requestPath, const Location* location, const Server* server) const;
    HttpResponse handleFile(const std::string& fullPath, const std::string& requestPath, const Location* location, const Server* server) const;
    HttpResponse createErrorResponse(int statusCode, const Location* location, const Server* server) const;

    HttpResponse handleCgi(const HttpRequest& request, const Location* location, const Server* server) const;

  public:
    HttpGetRequestHandler();
    HttpGetRequestHandler(const HttpGetRequestHandler& other);
    HttpGetRequestHandler& operator=(const HttpGetRequestHandler& other);
    virtual ~HttpGetRequestHandler();
};

#endif
