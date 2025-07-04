#ifndef ICONTROLLER_HPP
#define ICONTROLLER_HPP

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include <string>

class IController
{
  public:
    virtual ~IController()
    {
    }

    virtual HttpResponse handleGet(const HttpRequest& request) = 0;
    virtual HttpResponse handlePost(const HttpRequest& request) = 0;
    virtual HttpResponse handleDelete(const HttpRequest& request) = 0;

    virtual bool supportsMethod(const std::string& method) const = 0;
};

#endif
