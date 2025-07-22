#ifndef PATHHANDLER_HPP
#define PATHHANDLER_HPP

#include <string>
#include "webserv.hpp"

class PathHandler
{
  public:
    static std::string normalizeUrlPath(const std::string& path);
    static std::string normalizeFilePath(const std::string& path);

    static std::string joinPath(const std::string& basePath, const std::string& relativePath);
    static std::string joinFilePath(const std::string& basePath, const std::string& relativePath);
    static std::string getDirectory(const std::string& path);
    static std::string getFileName(const std::string& path);
    static std::string getExtension(const std::string& path);

    static bool        isPrefixMatch(const std::string& requestPath, const std::string& locationPath);
    static std::string getRelativePath(const std::string& requestPath, const std::string& locationPath);

    static std::string uriDecode(const std::string &src);
    static std::string uriEncode(const std::string &src);

};

#endif
