#include "HttpDeleteRequestHandler.hpp"
#include "../utils/PathHandler.hpp"
#include "FileSystemHandler.hpp"
#include "HttpResponse.hpp"

HttpDeleteRequestHandler::HttpDeleteRequestHandler() : AbstractHttpMethodHandler()
{
}

HttpDeleteRequestHandler::HttpDeleteRequestHandler(const HttpDeleteRequestHandler& other) : AbstractHttpMethodHandler(other)
{
}

HttpDeleteRequestHandler& HttpDeleteRequestHandler::operator=(const HttpDeleteRequestHandler& other)
{
    (void)other;
    return *this;
}

HttpDeleteRequestHandler::~HttpDeleteRequestHandler()
{
}

HttpResponse HttpDeleteRequestHandler::handleMethod(const HttpRequest& request, const Location* location, const Server* server) const
{
    if (location && !isMethodAllowed("DELETE", *location))
        return HttpResponse::methodNotAllowed();

    if (!location || !server)
        return HttpResponse::internalServerError();

    std::string requestPath = request.getUrl();
    if (requestPath.empty())
        requestPath = "/";

    std::string documentRoot = location->getRoot();
    if (documentRoot.empty())
        documentRoot = server->getRoot();

    std::string relativePath = PathHandler::getRelativePath(requestPath, location->getRoute());
    std::string fullPath = PathHandler::joinFilePath(documentRoot, relativePath);

    if (FileSystemHandler::isDirectory(fullPath))
        return handleDirectory(fullPath);
    else if (FileSystemHandler::isFile(fullPath))
        return handleFile(fullPath);
    else
        return HttpResponse::notFound();

    (void)request;
    (void)fullPath;

    return HttpResponse::response(204, "No Content", "", "text/plain");
}

HttpResponse HttpDeleteRequestHandler::handleDirectory(const std::string& fullPath) const
{
    if (!FileSystemHandler::directoryExists(fullPath))
        return HttpResponse::notFound();

    (void)fullPath;
    if (1 == 0) // TODO: false falta implementar el borrado
        return HttpResponse::response(204, "No Content", "", "text/plain");
    else
        return HttpResponse::internalServerError();
}

HttpResponse HttpDeleteRequestHandler::handleFile(const std::string& fullPath) const
{
    if (!FileSystemHandler::fileExists(fullPath))
        return HttpResponse::notFound();

    if (FileSystemHandler::deleteFile(fullPath))
        return HttpResponse::response(204, "No Content", "", "text/plain");
    else
        return HttpResponse::internalServerError();
}
