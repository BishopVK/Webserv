#include "HttpRequestHandler.hpp"
#include "ErrorPageGenerator.hpp"
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
        return ErrorPageGenerator::GenerateErrorResponse(HttpResponse::badRequest());

    std::string requestPath = request.getUrl();
    if (requestPath.empty())
        requestPath = "/";

    ServerConnection* serverConnection = client.getServerConnection();
    if (!serverConnection)
    {
        Logger::instance().error("No hay conexión de servidor disponible");
        return ErrorPageGenerator::GenerateErrorResponse(HttpResponse::internalServerError());
    }

    const Server* server = serverConnection->getServer();
    if (!server)
    {
        Logger::instance().error("No existen datos del servidor");
        return ErrorPageGenerator::GenerateErrorResponse(HttpResponse::internalServerError());
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
        return ErrorPageGenerator::GenerateErrorResponse(HttpResponse::methodNotAllowed());
}
