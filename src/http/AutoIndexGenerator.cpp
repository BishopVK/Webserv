#include "AutoIndexGenerator.hpp"
#include "../utils/PathHandler.hpp"
#include "FileSystemHandler.hpp"
#include <sstream>
#include <string>
#include <vector>

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
    std::vector<FileSystemHandler::DirectoryEntry> entries = FileSystemHandler::getDirectoryEntriesWithInfo(_physicalPath);
    if (entries.empty() && !FileSystemHandler::isDirectory(_physicalPath))
        return "<html><body><h1>404 Not Found</h1></body></html>";

    std::stringstream css;
    css << "<style>\n"
        << "body { font-family: sans-serif; margin: 2em; background: #fff; color: #000; }\n"
        << "h1 { font-size: 1.5em; margin-bottom: 1em; }\n"
        << "table { width: 100%; border-collapse: collapse; }\n"
        << "th, td { padding: 0.5em; text-align: left; border-bottom: 1px solid #ddd; }\n"
        << "a { text-decoration: none; color: #0366d6; }\n"
        << "a:hover { text-decoration: underline; }\n"
        << "</style>\n";

    std::stringstream html;
    html << "<!DOCTYPE html>\n<html>\n<head>\n"
         << "<title>Index of " << _requestPath << "</title>\n"
         << css.str() << "</head>\n<body>\n"
         << "<h1>Index of " << _requestPath << "</h1>\n"
         << "<table>\n"
         << "<thead><tr><th>Name</th><th>Size (MB)</th></tr></thead>\n"
         << "<tbody>\n";

    if (_requestPath != "/")
    {
        std::string parentPath = PathHandler::getDirectory(PathHandler::normalizeUrlPath(_requestPath));
        if (parentPath.empty() || parentPath == ".")
            parentPath = "/";
        html << "<tr><td><a href=\"" << parentPath << "\">..</a></td><td>-</td></tr>\n";
    }

    for (std::vector<FileSystemHandler::DirectoryEntry>::const_iterator it = entries.begin(); it != entries.end(); ++it)
    {
        std::string       webPath = PathHandler::joinPath(_requestPath, it->name);
        std::stringstream sizeText;

        if (it->type == FileSystemHandler::FILE)
        {
            double sizeMb = static_cast<double>(it->size) / (1024.0 * 1024.0);
            sizeText.precision(2);
            sizeText << std::fixed << sizeMb;
        }
        else
            sizeText << "-";

        html << "<tr>"
             << "<td><a href=\"" << webPath << "\">" << it->name << "</a></td>"
             << "<td>" << sizeText.str() << "</td>"
             << "</tr>\n";
    }

    html << "</tbody>\n</table>\n</body>\n</html>\n";
    return html.str();
}
