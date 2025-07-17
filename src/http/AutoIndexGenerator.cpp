#include "AutoIndexGenerator.hpp"
#include "../utils/PathHandler.hpp"
#include "FileSystemHandler.hpp"
#include <sstream>
#include <string>

AutoIndexGenerator::AutoIndexGenerator(const std::string& requestPath, const std::string& physicalPath)
    : _requestPath(requestPath), _physicalPath(physicalPath)
{
}

AutoIndexGenerator::AutoIndexGenerator(const AutoIndexGenerator& other) : _requestPath(other._requestPath), _physicalPath(other._physicalPath)
{
}

AutoIndexGenerator::~AutoIndexGenerator()
{
}

AutoIndexGenerator& AutoIndexGenerator::operator=(const AutoIndexGenerator& other)
{
    if (this != &other)
    {
        _requestPath = other._requestPath;
        _physicalPath = other._physicalPath;
    }
    return *this;
}

std::string AutoIndexGenerator::generateHtml() const
{
    std::vector<std::string> entries = FileSystemHandler::getDirectoryEntries(_physicalPath);
    if (entries.empty() && !FileSystemHandler::isDirectory(_physicalPath))
        return "<html><body><h1>404 Not Found</h1></body></html>";

    std::stringstream html;
    html << "<!DOCTYPE html>\n<html>\n<head>\n"
         << "<title>Index of " << _requestPath << "</title>\n"
         << "</head>\n<body>\n"
         << "<h1>Index of " << _requestPath << "</h1>\n"
         << "<ul>\n";

    if (_requestPath != "/")
    {
        std::string parentPath = PathHandler::getDirectory(PathHandler::normalizeUrlPath(_requestPath));
        if (parentPath.empty() || parentPath == ".")
            parentPath = "/";
        html << "<li><a href=\"" << parentPath << "\">..</a></li>\n";
    }

    for (std::vector<std::string>::const_iterator it = entries.begin(); it != entries.end(); ++it)
    {
        std::string webPath = PathHandler::joinPath(_requestPath, *it);
        html << "<li><a href=\"" << webPath << "\">" << *it << "</a></li>\n";
    }

    html << "</ul>\n</body>\n</html>\n";
    return html.str();
}
