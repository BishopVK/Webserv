#include "AbstractHttpMethodHandler.hpp"
#include <string>
#include <vector>

AbstractHttpMethodHandler::AbstractHttpMethodHandler()
{
}

AbstractHttpMethodHandler::AbstractHttpMethodHandler(const AbstractHttpMethodHandler& other)
{
    (void)other;
}

AbstractHttpMethodHandler& AbstractHttpMethodHandler::operator=(const AbstractHttpMethodHandler& other)
{
    (void)other;
    return *this;
}

AbstractHttpMethodHandler::~AbstractHttpMethodHandler()
{
}

HttpResponse AbstractHttpMethodHandler::handle(const HttpRequest& request, const Location* location, const Server* server) const
{
    return handleMethod(request, location, server);
}

bool AbstractHttpMethodHandler::isMethodAllowed(const std::string& method, const Location& location) const
{
    const std::vector<std::string>&          allowedMethods = location.getMethods();
    std::vector<std::string>::const_iterator it = allowedMethods.begin();
    for (; it != allowedMethods.end(); ++it)
    {
        if (*it == method)
            return true;
    }

    return false;
}

bool AbstractHttpMethodHandler::isCgiRequest(const std::string& requestPath, const Location* location) const
{
    if (!location)
        return false;

    const std::string& cgiExtension = location->getCgiExtension();
    if (cgiExtension.empty())
        return false;

    size_t extensionPos = requestPath.rfind(cgiExtension);
    if (extensionPos == std::string::npos)
        return false;
    if (extensionPos + cgiExtension.length() != requestPath.length())
        return false;

    return true;
}

std::vector<std::string> AbstractHttpMethodHandler::getIndexFiles(const Location* location, const Server* server) const
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

std::string AbstractHttpMethodHandler::getErrorPage(int statusCode, const Location* location, const Server* server) const
{
    if (location)
    {
        std::string locationErrorPage = location->getErrorPage(statusCode);
        if (!locationErrorPage.empty())
            return locationErrorPage;
    }

    return server->getErrorPage(statusCode);
}
