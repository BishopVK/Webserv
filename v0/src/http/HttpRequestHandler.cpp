#include "HttpRequestHandler.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

HttpRequestHandler::HttpRequestHandler() : _registry()
{
}

HttpRequestHandler::HttpRequestHandler(const HttpRequestHandler& other) : _registry(other._registry)
{
}

HttpRequestHandler& HttpRequestHandler::operator=(const HttpRequestHandler& other)
{
    if (this != &other)
        _registry = other._registry;
    return *this;
}

HttpRequestHandler::~HttpRequestHandler()
{
}

HttpResponse HttpRequestHandler::handle(const HttpRequest& request)
{
    if (!request.isValid())
        return HttpResponse::badRequest();

    return _registry.processRequest(request);
}
