#include "HttpRequestHandler.hpp"
#include "AutoIndexGenerator.hpp"
#include "ClientConnection.hpp"
#include "ContentTypeManager.hpp"
#include "FileSystemHandler.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Location.hpp"
#include "LocationMatcher.hpp"
#include "Logger.hpp"
#include "../utils/PathHandler.hpp"
#include "Server.hpp"
#include <string>
#include <vector>

HttpRequestHandler::HttpRequestHandler()
{
}

HttpRequestHandler::HttpRequestHandler(const HttpRequestHandler& other)
{
    (void)other;
}

HttpRequestHandler& HttpRequestHandler::operator=(const HttpRequestHandler& other)
{
    (void)other;
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

    if (matchedLocation)
        return handleWithLocation(*matchedLocation, requestPath, server);
    else
        return handleWithServerDefaults(requestPath, server);
}

HttpResponse HttpRequestHandler::handleWithLocation(const Location& location, const std::string& requestPath, const Server* server) const
{
    const Config::ReturnData& returnData = location.getReturnData();
    if (returnData.code >= 300 && returnData.code < 400 && !returnData.text.empty())
        return HttpResponse::redirect(returnData.text, returnData.code);

    std::string relativePath = PathHandler::getRelativePath(requestPath, location.getRoute());
    std::string documentRoot = location.getRoot();
    if (documentRoot.empty())
        documentRoot = server->getRoot();

    std::string fullPath = PathHandler::joinFilePath(documentRoot, relativePath);

    return handleResource(fullPath, requestPath, &location, server);
}

HttpResponse HttpRequestHandler::handleWithServerDefaults(const std::string& requestPath, const Server* server) const
{
    std::string documentRoot = server->getRoot();
    if (documentRoot.empty())
        documentRoot = "./var/www";

    std::string fullPath = PathHandler::joinFilePath(documentRoot, requestPath);

    return handleResource(fullPath, requestPath, NULL, server);
}

HttpResponse HttpRequestHandler::handleResource(const std::string& fullPath, const std::string& requestPath, const Location* location, const Server* server) const
{
    FileSystemHandler::ResourceType resourceType = FileSystemHandler::getResourceType(fullPath);

    switch (resourceType)
    {
    case FileSystemHandler::DIRECTORY:
        return handleDirectory(fullPath, requestPath, location, server);

    case FileSystemHandler::FILE:
        return handleFile(fullPath, requestPath, location, server);

    case FileSystemHandler::NOT_FOUND:
    default:
        return createErrorResponse(404, location, server);
    }
}

HttpResponse HttpRequestHandler::handleDirectory(const std::string& fullPath, const std::string& requestPath, const Location* location, const Server* server) const
{
    std::vector<std::string> indexFiles = getIndexFiles(location, server);

    for (std::vector<std::string>::const_iterator it = indexFiles.begin(); it != indexFiles.end(); ++it)
    {
        std::string indexPath = PathHandler::joinFilePath(fullPath, *it);
        if (FileSystemHandler::isFile(indexPath))
            return handleFile(indexPath, requestPath, location, server);
    }

    bool autoindex = location ? location->getAutoindex() : server->getAutoindex();

    if (autoindex)
    {
        std::string autoindexHtml = AutoIndexGenerator::generateHtml(requestPath, fullPath);
        return HttpResponse::ok(autoindexHtml, "text/html");
    }
    else
        return createErrorResponse(404, location, server);
}

HttpResponse HttpRequestHandler::handleFile(const std::string& fullPath, const std::string& /* requestPath */, const Location* location, const Server* server) const
{
    std::string content = FileSystemHandler::getFileContent(fullPath);
    if (content.empty())
        return createErrorResponse(404, location, server);

    std::string contentType = ContentTypeManager::getContentType(fullPath);

    return HttpResponse::ok(content, contentType);
}

HttpResponse HttpRequestHandler::createErrorResponse(int statusCode, const Location* location, const Server* server) const
{
    std::string documentRoot = location->getRoot();
    if (documentRoot.empty())
        documentRoot = server->getRoot();

    std::string errorPage = getErrorPage(statusCode, location, server);
    std::string errorPagePath = PathHandler::joinFilePath(documentRoot, errorPage);
    if (!errorPage.empty() && FileSystemHandler::isFile(errorPagePath))
    {
        std::string content = FileSystemHandler::getFileContent(errorPagePath);
        if (!content.empty())
        {
            std::string  contentType = ContentTypeManager::getContentType(errorPagePath);
            HttpResponse response = HttpResponse::response(statusCode, "", content, contentType);
            return response;
        }
    }

    switch (statusCode)
    {
    case 403:
        return HttpResponse::response(403, "403 Forbidden", "", "text/html");
    case 404:
        return HttpResponse::notFound();
    case 500:
    default:
        return HttpResponse::internalServerError();
    }
}

std::vector<std::string> HttpRequestHandler::getIndexFiles(const Location* location, const Server* server) const
{
    if (location)
    {
        std::vector<std::string> locationIndexFiles = location->getIndexFiles();
        if (!locationIndexFiles.empty())
            return locationIndexFiles;
    }

    std::vector<std::string> serverIndexFiles = server->getIndexFiles();
    if (!serverIndexFiles.empty())
        return serverIndexFiles;

    std::vector<std::string> defaultIndexFiles;
    defaultIndexFiles.push_back("index.html");
    defaultIndexFiles.push_back("index.htm");
    return defaultIndexFiles;
}

std::string HttpRequestHandler::getErrorPage(int statusCode, const Location* location, const Server* server) const
{
    if (location)
    {
        std::string locationErrorPage = location->getErrorPage(statusCode);
        if (!locationErrorPage.empty())
            return locationErrorPage;
    }

    return server->getErrorPage(statusCode);
}
