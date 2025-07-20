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

// ADDED BY DANI (original más abajo)
/* HttpResponse HttpDeleteRequestHandler::handleMethod(const HttpRequest& request, const Location* location, const Server* server) const
{
    // 1) Comprobaciones de método permitido
    if (location && !isMethodAllowed("DELETE", *location))
        return HttpResponse::methodNotAllowed();
    if (!location || !server)
        return HttpResponse::internalServerError();

    // 2) Si la ruta configurada es "/delete/", procesamos body form-url-encoded
    if (location->getRoute() == "/delete/") {
        // Obtener cuerpo de la petición
        std::string body = request.getBody();

        // Parse URL-encoded en C++98
        std::map<std::string, std::string> params =
            PathHandler::parseUrlEncoded(body);

        // Validar parámetro "file"
        if (params.find("file") == params.end() || params["file"].empty())
            return HttpResponse::badRequest();  // 400 Bad Request

        // Sanitizar nombre de fichero
        std::string filename = PathHandler::getBasename(params["file"]);

        // Construir ruta absoluta al directorio de uploads
        std::string uploadDir = server->getRoot() + "/upload/uploads";
        std::string fullPath = PathHandler::joinFilePath(uploadDir, filename);

        // Delegar al método que borra ficheros
        return handleFile(fullPath);
    }

    // 3) Si no es el endpoint /delete/, caemos en el flujo genérico
    std::string requestPath = request.getUrl();
    if (requestPath.empty())
        requestPath = "/";

    // Determinar documentRoot
    std::string documentRoot;
    if (location->getRoot().empty())
        documentRoot = server->getRoot();
    else
        documentRoot = location->getRoot();

    // Obtener ruta relativa y ensamblar path completo
    std::string relativePath =
        PathHandler::getRelativePath(requestPath, location->getRoute());
    std::string staticFullPath =
        PathHandler::joinFilePath(documentRoot, relativePath);

    // Comprobar si es directorio o fichero
    if (FileSystemHandler::isDirectory(staticFullPath)) {
        return handleDirectory(staticFullPath);
    }
    else if (FileSystemHandler::isFile(staticFullPath)) {
        return handleFile(staticFullPath);
    }
    else {
        return HttpResponse::notFound();
    }
} */

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
