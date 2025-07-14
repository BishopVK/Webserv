#include "HttpPostRequestHandler.hpp"

HttpPostRequestHandler::HttpPostRequestHandler() : AbstractHttpMethodHandler()
{
}

HttpPostRequestHandler::HttpPostRequestHandler(const HttpPostRequestHandler& other) : AbstractHttpMethodHandler(other)
{
}

HttpPostRequestHandler& HttpPostRequestHandler::operator=(const HttpPostRequestHandler& other)
{
    (void)other;
    return *this;
}

HttpPostRequestHandler::~HttpPostRequestHandler()
{
}

HttpResponse HttpPostRequestHandler::handleMethod(const HttpRequest& request, const Location* location, const Server* server) const
{
    if (location && !isMethodAllowed("POST", *location))
        return HttpResponse::methodNotAllowed();

    std::string requestPath = request.getUrl();
    if (requestPath.empty())
        requestPath = "/";

    // TODO: Implementar el manejo de solicitudes POST
    (void)request;
    (void)requestPath;
    (void)location;
    (void)server;

    return HttpResponse::notImplemented("POST method handling not implemented yet");
}
