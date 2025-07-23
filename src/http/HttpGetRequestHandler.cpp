#include "HttpGetRequestHandler.hpp"
#include "AutoIndexGenerator.hpp"
#include "CgiHandler.hpp"
#include "ContentTypeManager.hpp"
#include "ErrorPageGenerator.hpp"
#include "FileSystemHandler.hpp"
#include "HttpResponse.hpp"
#include "PathHandler.hpp"
#include <string>

HttpGetRequestHandler::HttpGetRequestHandler() : AbstractHttpMethodHandler()
{
}

HttpGetRequestHandler::HttpGetRequestHandler(const HttpGetRequestHandler& other) : AbstractHttpMethodHandler(other)
{
}

HttpGetRequestHandler& HttpGetRequestHandler::operator=(const HttpGetRequestHandler& other)
{
    (void)other;
    return *this;
}

HttpGetRequestHandler::~HttpGetRequestHandler()
{
}

HttpResponse HttpGetRequestHandler::handleMethod(const HttpRequest& request, const Location* location, const Server* server) const
{
    if (location && !isMethodAllowed("GET", *location))
        return createErrorResponse(405, location, server);

    std::string requestPath = request.getUrl();
    if (requestPath.empty())
        requestPath = "/";

    if (CgiHandler::isCgiRequest(requestPath, location))
        return  CgiHandler::execute(request, location, server);

    if (location)
        return handleWithLocation(*location, requestPath, server);
    else
        return handleWithServerDefaults(requestPath, server);
}

HttpResponse HttpGetRequestHandler::handleWithLocation(const Location& location, const std::string& requestPath, const Server* server) const
{
    const Config::ReturnData& returnData = location.getReturnData();
    if (returnData.code >= 300 && returnData.code < 400 && !returnData.text.empty())
        return HttpResponse::redirect(returnData.text, returnData.code);

    std::string relativePath = PathHandler::getRelativePath(requestPath, location.getRoute());
    std::string documentRoot = location.getRoot();
    if (documentRoot.empty())
        documentRoot = server->getRoot();

    std::string encodedPath = PathHandler::uriDecode(relativePath); // DB Uri Decoder
    std::string fullPath = PathHandler::joinFilePath(documentRoot, encodedPath);

    return handleResource(fullPath, requestPath, &location, server);
}

HttpResponse HttpGetRequestHandler::handleWithServerDefaults(const std::string& requestPath, const Server* server) const
{
    std::string documentRoot = server->getRoot();
    if (documentRoot.empty())
        documentRoot = "./var/www";

    std::string encodedPath = PathHandler::uriDecode(requestPath); // DB Uri Decoder
    std::string fullPath = PathHandler::joinFilePath(documentRoot, encodedPath);

    return handleResource(fullPath, requestPath, NULL, server);
}

HttpResponse HttpGetRequestHandler::handleResource(const std::string& fullPath, const std::string& requestPath, const Location* location, const Server* server) const
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

HttpResponse HttpGetRequestHandler::handleDirectory(const std::string& fullPath, const std::string& requestPath, const Location* location, const Server* server) const
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
        AutoIndexGenerator autoIndexGenerator(requestPath, fullPath);
        std::string autoindexHtml = autoIndexGenerator.generateHtml();

        return HttpResponse::ok(autoindexHtml, "text/html");
    }
    else
        return createErrorResponse(404, location, server);
}

HttpResponse HttpGetRequestHandler::handleFile(const std::string& fullPath, const std::string& /* requestPath */, const Location* location, const Server* server) const
{
    std::string content = FileSystemHandler::getFileContent(fullPath);

    if (content.empty())
        return createErrorResponse(404, location, server);

    std::string contentType = ContentTypeManager::getContentType(fullPath);
    //std::cerr <<"Serving: " << fullPath << " with Content-Type: " << contentType << std::endl; // DB

    return HttpResponse::ok(content, contentType);
}

HttpResponse HttpGetRequestHandler::createErrorResponse(int statusCode, const Location* location, const Server* server) const
{
    std::string documentRoot = location ? location->getRoot() : "";
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
            return ErrorPageGenerator::GenerateErrorResponse(HttpResponse::notFound());
        case 405:
            return ErrorPageGenerator::GenerateErrorResponse(HttpResponse::methodNotAllowed());
        case 500:
        default:
            return ErrorPageGenerator::GenerateErrorResponse(HttpResponse::internalServerError());
    }
}
