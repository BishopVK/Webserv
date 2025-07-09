#include "HttpRequestHandler.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Logger.hpp"
#include <cstddef>
#include <dirent.h>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

std::map<std::string, std::string> HttpRequestHandler::_contentTypeMap;

HttpRequestHandler::HttpRequestHandler() : _registry()
{
    if (_contentTypeMap.empty())
    {
        _contentTypeMap["html"] = "text/html";
        _contentTypeMap["htm"] = "text/html";
        _contentTypeMap["css"] = "text/css";
        _contentTypeMap["js"] = "application/javascript";
        _contentTypeMap["json"] = "application/json";
        _contentTypeMap["png"] = "image/png";
        _contentTypeMap["jpg"] = "image/jpeg";
        _contentTypeMap["jpeg"] = "image/jpeg";
        _contentTypeMap["gif"] = "image/gif";
        _contentTypeMap["svg"] = "image/svg+xml";
        _contentTypeMap["pdf"] = "application/pdf";
        _contentTypeMap["txt"] = "text/plain";
        _contentTypeMap["xml"] = "application/xml";
    }
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
    {
        Logger::instance().warning("Request invalida");
        return HttpResponse::badRequest();
    }

    std::string url = request.getUrl();
    if (url.empty())
        url = "/";

    std::string fullPath = "./var/www" + url;

    ResourceType resourceType = getResourceType(fullPath);

    if (resourceType == DIRECTORY)
    {
        Logger::instance().debug("Leyendo directorio: " + fullPath);
        return HttpResponse::ok(generateAutoIndexHtml(fullPath), "text/html");
    }
    else if (resourceType == FILE)
    {
        Logger::instance().debug("Fichero: " + fullPath);
        std::string content = getFileContent(fullPath);
        if (content.empty())
        {
            Logger::instance().warning("No se ha podido leer el contenido del archivo: " + fullPath);
            return HttpResponse::notFound();
        }
        std::string contentType = getContentType(fullPath);
        Logger::instance().debug("Fichero con tipo: " + contentType);
        return HttpResponse::ok(content, contentType);
    }
    else if (resourceType == NOT_FOUND)
    {
        Logger::instance().warning("Recurso no encontrado: " + fullPath);
        return HttpResponse::notFound();
    }

    Logger::instance().debug("Fallo al procesar la solicitud.");
    return _registry.processRequest(request);
}

std::string HttpRequestHandler::generateAutoIndexHtml(const std::string& path) const
{
    std::string html = "<html><body><h1>Index of " + path + "</h1><ul>";
    DIR*        dir = opendir(path.c_str());
    if (!dir)
        return "<h1>404 Not Found</h1>";

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (std::string(entry->d_name) == ".")
            continue;
        Logger::instance().debug("Entry found: " + std::string(entry->d_name) + " in path: " + path);

        std::string       web_path = "";
        const std::string prefix = "./var/www";

        // Pasar de "./var/www/ruta" a "ruta"
        if (path.length() >= prefix.length() && path.substr(0, prefix.length()) == prefix)
            web_path = path.substr(prefix.length());

        // web_path ha de terminar con '/'
        if (!web_path.empty() && web_path[web_path.length() - 1] != '/')
            web_path += "/";

        // Unir el nombre del archivo al web_path
        std::string href = web_path + std::string(entry->d_name);

        // Asegurarse de que el href comience con '/'
        if (href.empty() || href[0] != '/')
            href = "/" + href;

        // Agregar el enlace al HTML
        html += "<li><a href=\"" + href + "\">" + std::string(entry->d_name) + "</a></li>";
    }
    closedir(dir);
    html += "</ul></body></html>";
    return html;
}

std::string HttpRequestHandler::getFileContent(const std::string& path) const
{
    std::ifstream file(path.c_str());
    if (!file.is_open())
    {
        Logger::instance().error("Failed to open file: " + path);
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}

HttpRequestHandler::ResourceType HttpRequestHandler::getResourceType(const std::string& path) const
{
    struct stat pathStat;
    if (stat(path.c_str(), &pathStat) != 0)
        return NOT_FOUND;

    if (S_ISDIR(pathStat.st_mode))
        return DIRECTORY;
    else if (S_ISREG(pathStat.st_mode))
        return FILE;

    return NOT_FOUND;
}

std::string HttpRequestHandler::getContentType(const std::string& path) const
{
    size_t dotPos = path.find_last_of('.');
    if (dotPos == std::string::npos)
        return "text/plain";

    std::string extension = path.substr(dotPos + 1);

    std::map<std::string, std::string>::const_iterator it = _contentTypeMap.find(extension);
    if (it != _contentTypeMap.end())
        return it->second;

    return "text/plain";
}
