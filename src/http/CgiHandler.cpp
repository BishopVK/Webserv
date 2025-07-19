#include "CgiHandler.hpp"
#include "../cgis/Cgis.hpp"
#include "../utils/PathHandler.hpp"
#include "FileSystemHandler.hpp"
#include "Logger.hpp"
#include <sstream>

CgiHandler::CgiHandler()
{
}

CgiHandler::CgiHandler(const CgiHandler& other)
{
    (void)other;
}

CgiHandler& CgiHandler::operator=(const CgiHandler& other)
{
    (void)other;
    return *this;
}

CgiHandler::~CgiHandler()
{
}

bool CgiHandler::isCgiLocation(const Location* location)
{
    if (!location)
        return false;

    const std::string& cgiExtension = location->getCgiExtension();
    return !cgiExtension.empty();
}

bool CgiHandler::isCgiRequest(const std::string& requestPath, const Location* location)
{
    if (!isCgiLocation(location))
        return false;

    const std::string& cgiExtension = location->getCgiExtension();

    if (hasValidCgiExtension(requestPath, cgiExtension))
        return true;

    std::vector<std::string> indexFiles = location->getIndexFiles();
    for (std::vector<std::string>::const_iterator it = indexFiles.begin(); it != indexFiles.end(); ++it)
    {
        if (hasValidCgiExtension(*it, cgiExtension))
            return true;
    }

    return false;
}

bool CgiHandler::hasValidCgiExtension(const std::string& filePath, const std::string& cgiExtension)
{
    if (cgiExtension.empty() || filePath.empty())
        return false;

    size_t extensionPos = filePath.rfind(cgiExtension);
    if (extensionPos == std::string::npos)
        return false;

    return (extensionPos + cgiExtension.length() == filePath.length());
}

std::string CgiHandler::getCgiFilePath(const HttpRequest& request, const Location* location, const Server* server)
{
    std::string requestPath = request.getUrl();
    if (requestPath.empty())
        requestPath = "/";

    std::string documentRoot = location->getRoot();
    if (documentRoot.empty())
        documentRoot = server->getRoot();

    std::string relativePath = PathHandler::getRelativePath(requestPath, location->getRoute());
    std::string fullPath = PathHandler::joinFilePath(documentRoot, relativePath);

    if (FileSystemHandler::isFile(fullPath) && hasValidCgiExtension(requestPath, location->getCgiExtension()))
        return fullPath;

    if (FileSystemHandler::isDirectory(fullPath))
    {
        std::string defaultCgiFile = getDefaultCgiFile(location, server);
        if (!defaultCgiFile.empty())
        {
            std::string defaultCgiPath = PathHandler::joinFilePath(fullPath, defaultCgiFile);
            if (FileSystemHandler::isFile(defaultCgiPath))
                return defaultCgiPath;
        }
    }

    return "";
}

std::string CgiHandler::getDefaultCgiFile(const Location* location, const Server* server)
{
    if (!location)
        return "";

    const std::string& cgiExtension = location->getCgiExtension();
    if (cgiExtension.empty())
        return "";

    std::vector<std::string> indexFiles = location->getIndexFiles();
    for (std::vector<std::string>::const_iterator it = indexFiles.begin(); it != indexFiles.end(); ++it)
    {
        if (hasValidCgiExtension(*it, cgiExtension))
            return *it;
    }

    if (server)
    {
        std::vector<std::string> serverIndexFiles = server->getIndexFiles();
        for (std::vector<std::string>::const_iterator it = serverIndexFiles.begin(); it != serverIndexFiles.end(); ++it)
        {
            if (hasValidCgiExtension(*it, cgiExtension))
                return *it;
        }
    }

    return "";
}

HttpResponse CgiHandler::execute(const HttpRequest& request, const Location* location, const Server* server)
{
    if (!isCgiRequest(request.getUrl(), location))
        return HttpResponse::internalServerError();

    std::string cgiFilePath = getCgiFilePath(request, location, server);
    if (cgiFilePath.empty())
        return HttpResponse::notFound();

    std::string directoryPath = PathHandler::getDirectory(cgiFilePath);
    std::string fileName = PathHandler::getFileName(cgiFilePath);

    std::string method = request.getMethod();
    std::string contentType = request.getHeader("Content-Type");
    std::string contentLength;
    std::string body;
    std::string boundary;

    if (method == "GET")
    {
        body = request.getRawParameters();
        size_t            contentLengthValue = body.size();
        std::stringstream contentLengthStream;
        contentLengthStream << contentLengthValue;
        contentLength = contentLengthStream.str();
    }
    else if (method == "POST")
    {
        body = request.getBody();
        contentLength = request.getHeader("Content-Length");

        if (contentType.find("multipart/form-data") != std::string::npos)
        {
            size_t boundaryPos = contentType.find("boundary=");
            if (boundaryPos != std::string::npos)
            {
                boundary = contentType.substr(boundaryPos + 9);
                size_t semicolonPos = boundary.find(';');
                if (semicolonPos != std::string::npos)
                    boundary = boundary.substr(0, semicolonPos);
            }
            contentType = contentType.substr(0, contentType.find(';'));
        }

        if (contentLength.empty())
        {
            std::stringstream contentLengthStream;
            contentLengthStream << body.size();
            contentLength = contentLengthStream.str();
        }
    }
    else
    {
        return HttpResponse::methodNotAllowed();
    }

    try
    {
        Cgis         cgiHandler(method, directoryPath + "/", fileName, contentType, boundary, contentLength, body, 0);
        HttpResponse response = cgiHandler.execute();

        return response;
    }
    catch (const std::exception& e)
    {
        Logger::instance().error("Ejecucion de CGI fallida" + std::string(e.what()));
        return HttpResponse::internalServerError();
    }
}
