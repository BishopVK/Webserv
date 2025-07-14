#ifndef HTTP_DELETE_REQUEST_HANDLER_HPP
#define HTTP_DELETE_REQUEST_HANDLER_HPP

#include "AbstractHttpMethodHandler.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include <string>

class HttpDeleteRequestHandler : public AbstractHttpMethodHandler
{
  private:
    virtual HttpResponse handleMethod(const HttpRequest& request, const Location* location, const Server* server) const;

    HttpResponse handleDirectory(const std::string& fullPath) const;
    HttpResponse handleFile(const std::string& fullPath) const;

  public:
    HttpDeleteRequestHandler();
    HttpDeleteRequestHandler(const HttpDeleteRequestHandler& other);
    HttpDeleteRequestHandler& operator=(const HttpDeleteRequestHandler& other);
    virtual ~HttpDeleteRequestHandler();
};

#endif
