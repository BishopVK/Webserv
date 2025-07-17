#ifndef ABSTRACT_HTTP_METHOD_HANDLER_HPP
#define ABSTRACT_HTTP_METHOD_HANDLER_HPP

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include <string>
#include <vector>

class AbstractHttpMethodHandler
{
  protected:
    virtual HttpResponse handleMethod(const HttpRequest& request, const Location* location, const Server* server) const = 0;

    bool isMethodAllowed(const std::string& method, const Location& location) const;

    std::vector<std::string> getIndexFiles(const Location* location, const Server* server) const;
    std::string              getErrorPage(int statusCode, const Location* location, const Server* server) const;

  public:
    AbstractHttpMethodHandler();
    AbstractHttpMethodHandler(const AbstractHttpMethodHandler& other);
    AbstractHttpMethodHandler& operator=(const AbstractHttpMethodHandler& other);
    virtual ~AbstractHttpMethodHandler();

    HttpResponse handle(const HttpRequest& request, const Location* location, const Server* server) const;
};

#endif
