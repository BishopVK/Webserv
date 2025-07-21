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

    std::stringstream css;
    css << "<style>\n"
        << "html, body { height: 100%; font-family: 'Segoe UI', sans-serif; line-height: 1.6; background-color: #f4f4f4; color: #333; }\n"
        << "h1 { margin-bottom: 2rem; font-size: 2rem; color: #05053a; }\n"
        << "li { transition: 0.5s ease; width: fit-content; }\n"
        << "li:hover { font-weight: 800; text-decoration: underline; transform: translateX(10px); }\n"
        << "li a { text-decoration: none; color: #05053a; font-size: 1.2rem; }\n"
        << "</style>\n";

    std::stringstream html;
    html << "<!DOCTYPE html>\n<html>\n<head>\n"
         << "<link rel=\"stylesheet\" href=\"/autoindex.css\">\n"
         << "<title>Index of " << _requestPath << "</title>\n"
        //  << css.str()
         << "</head>\n<body>\n"
         << "<div class=\"container\">"
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

    html << "</ul>\n</div>\n</body>\n</html>\n";
    return html.str();
}
