#include "HttpRequestHandler.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Location.hpp"
#include "LocationMatcher.hpp"
#include "Logger.hpp"
#include "Server.hpp"
#include <string>
#include <vector>

HttpRequestHandler::HttpRequestHandler()
{
}

HttpRequestHandler::HttpRequestHandler(const HttpRequestHandler& other)
    : _getHandler(other._getHandler), _postHandler(other._postHandler), _deleteHandler(other._deleteHandler)
{
}

HttpRequestHandler& HttpRequestHandler::operator=(const HttpRequestHandler& other)
{
    if (this != &other)
    {
        _getHandler = other._getHandler;
        _postHandler = other._postHandler;
        _deleteHandler = other._deleteHandler;
    }
    return *this;
}

HttpRequestHandler::~HttpRequestHandler()
{
}

HttpResponse HttpRequestHandler::handle(const HttpRequest& request, const ClientConnection& client) const
{
    if (!request.isValid())
        return HttpResponse::badRequest();

    std::string requestPath = request.getUrl();
    if (requestPath.empty())
        requestPath = "/";

    const Server* server = client.getServerConnection()->getServer();
    if (!server)
    {
        Logger::instance().error("No existen datos del servidor");
        return HttpResponse::internalServerError();
    }

    std::vector<Location> locations = server->getLocations();
    const Location*       matchedLocation = LocationMatcher::findBestMatch(requestPath, locations);

    Logger::instance().info("Metodo: " + request.getMethod() + ", URL: " + requestPath);

    if (request.getMethod() == "GET")
        return _getHandler.handle(request, matchedLocation, server);
    else if (request.getMethod() == "POST")
        return _postHandler.handle(request, matchedLocation, server);
    else if (request.getMethod() == "DELETE")
        return _deleteHandler.handle(request, matchedLocation, server);
    else
        return HttpResponse::methodNotAllowed();
}
