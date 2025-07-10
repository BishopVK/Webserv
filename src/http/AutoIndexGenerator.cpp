#include "AutoIndexGenerator.hpp"
#include "../utils/PathHandler.hpp"
#include "FileSystemHandler.hpp"
#include <sstream>

std::string AutoIndexGenerator::generateHtml(const std::string& requestPath, const std::string& physicalPath)
{
    std::vector<std::string> entries = FileSystemHandler::getDirectoryEntries(physicalPath);
    if (entries.empty() && !FileSystemHandler::isDirectory(physicalPath))
        return "<html><body><h1>404 Not Found</h1></body></html>";

    std::stringstream html;
    html << "<!DOCTYPE html>\n<html>\n<head>\n"
         << "<title>Index of " << requestPath << "</title>\n"
         << "</head>\n<body>\n"
         << "<h1>Index of " << requestPath << "</h1>\n"
         << "<ul>\n";

    if (requestPath != "/")
    {
        std::string parentPath = PathHandler::getDirectory(PathHandler::normalizeUrlPath(requestPath));
        if (parentPath.empty() || parentPath == ".")
            parentPath = "/";
        html << "<li><a href=\"" << parentPath << "\">..</a></li>\n";
    }

    for (std::vector<std::string>::const_iterator it = entries.begin(); it != entries.end(); ++it)
    {
        std::string webPath = PathHandler::joinPath(requestPath, *it);
        html << "<li><a href=\"" << webPath << "\">" << *it << "</a></li>\n";
    }

    html << "</ul>\n</body>\n</html>\n";
    return html.str();
}
