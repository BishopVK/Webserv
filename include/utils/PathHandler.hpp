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

    // ADDED BY DANI
    // Parsea un body "a=1&b=2" en un map<clave,valor>
    static std::map<std::string, std::string> parseUrlEncoded(const std::string& body);

    // Decodifica %XX y + → espacio
    static std::string urlDecode(const std::string& str);

    // Obtiene sólo el nombre de fichero de una ruta
    static std::string getBasename(const std::string& path);
};

#endif
