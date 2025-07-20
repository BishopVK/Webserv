#include "PathHandler.hpp"

std::string PathHandler::normalizeUrlPath(const std::string& path)
{
    if (path.empty())
        return "/";

    std::string result = path;

    if (result[0] != '/')
        result = "/" + result;

    if (result.length() > 1 && result[result.length() - 1] == '/')
        result = result.substr(0, result.length() - 1);

    return result;
}

std::string PathHandler::normalizeFilePath(const std::string& path)
{
    if (path.empty())
        return "./";

    std::string result = path;

    if (result.length() >= 2 && result.substr(0, 2) == "./")
        return result;

    if (result[0] == '/')
        result = "." + result;
    else
        result = "./" + result;

    return result;
}

std::string PathHandler::joinPath(const std::string& basePath, const std::string& relativePath)
{
    if (basePath.empty() && relativePath.empty())
        return "/";
    if (basePath.empty())
        return normalizeUrlPath(relativePath);
    if (relativePath.empty())
        return normalizeUrlPath(basePath);

    std::string result = basePath;

    if (result[result.length() - 1] != '/' && relativePath[0] != '/')
        result += "/";
    else if (result[result.length() - 1] == '/' && relativePath[0] == '/')
        result = result.substr(0, result.length() - 1);

    result += relativePath;
    return normalizeUrlPath(result);
}

std::string PathHandler::joinFilePath(const std::string& basePath, const std::string& relativePath)
{
    if (basePath.empty() && relativePath.empty())
        return "./";
    if (basePath.empty())
        return normalizeFilePath(relativePath);
    if (relativePath.empty())
        return normalizeFilePath(basePath);

    std::string result = basePath;

    if (result[result.length() - 1] != '/' && relativePath[0] != '/')
        result += "/";
    else if (result[result.length() - 1] == '/' && relativePath[0] == '/')
        result = result.substr(0, result.length() - 1);

    result += relativePath;
    return normalizeFilePath(result);
}

std::string PathHandler::getDirectory(const std::string& path)
{
    size_t lastSlash = path.find_last_of('/');
    if (lastSlash == std::string::npos)
        return ".";

    if (lastSlash == 0)
        return "/";

    return path.substr(0, lastSlash);
}

std::string PathHandler::getFileName(const std::string& path)
{
    size_t lastSlash = path.find_last_of('/');
    if (lastSlash == std::string::npos)
        return path;

    return path.substr(lastSlash + 1);
}

std::string PathHandler::getExtension(const std::string& path)
{
    std::string fileName = getFileName(path);
    size_t      lastDot = fileName.find_last_of('.');

    if (lastDot == std::string::npos || lastDot == 0)
        return "";

    return fileName.substr(lastDot);
}

bool PathHandler::isPrefixMatch(const std::string& requestPath, const std::string& locationPath)
{
    std::string normalizedRequest = normalizeUrlPath(requestPath);
    std::string normalizedLocation = normalizeUrlPath(locationPath);

    if (normalizedLocation == "/")
        return true;

    if (normalizedRequest.length() < normalizedLocation.length())
        return false;

    if (normalizedRequest.substr(0, normalizedLocation.length()) != normalizedLocation)
        return false;

    if (normalizedRequest.length() == normalizedLocation.length())
        return true;

    char nextChar = normalizedRequest[normalizedLocation.length()];
    return nextChar == '/';
}

std::string PathHandler::getRelativePath(const std::string& requestPath, const std::string& locationPath)
{
    std::string normalizedRequest = normalizeUrlPath(requestPath);
    std::string normalizedLocation = normalizeUrlPath(locationPath);

    if (!isPrefixMatch(normalizedRequest, normalizedLocation))
        return "";

    if (normalizedLocation == "/")
        return normalizedRequest;

    if (normalizedRequest.length() <= normalizedLocation.length())
        return "/";

    std::string relativePath = normalizedRequest.substr(normalizedLocation.length());
    if (relativePath.empty() || relativePath[0] != '/')
        relativePath = "/" + relativePath;

    return relativePath;
}

// ADDED BY DANI
static int hexValue(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

std::map<std::string,std::string>   PathHandler::parseUrlEncoded(const std::string& body) {
    std::map<std::string, std::string> m;
    std::string::size_type start = 0;
    while (start < body.size()) {
        std::string::size_type eq = body.find('=', start);
        if (eq == std::string::npos) break;
        std::string key = urlDecode(body.substr(start, eq - start));

        std::string::size_type amp = body.find('&', eq + 1);
        std::string valEnc;
        if (amp == std::string::npos) {
            valEnc = body.substr(eq + 1);
            start = body.size();
        } else {
            valEnc = body.substr(eq + 1, amp - eq - 1);
            start = amp + 1;
        }

        std::string value = urlDecode(valEnc);
        m[key] = value;
    }
    return m;
}

std::string PathHandler::getBasename(const std::string& path) {
    std::string::size_type pos = path.find_last_of("/\\");
    if (pos == std::string::npos)
        return path;
    return path.substr(pos + 1);
}

std::string PathHandler::urlDecode(const std::string& str) {
    std::string result;
    result.reserve(str.size());
    for (std::string::size_type i = 0; i < str.size(); ++i) {
        char c = str[i];
        if (c == '+') {
            result += ' ';
        }
        else if (c == '%' && i + 2 < str.size()) {
            int hi = hexValue(str[i + 1]);
            int lo = hexValue(str[i + 2]);
            if (hi >= 0 && lo >= 0) {
                result += static_cast<char>((hi << 4) | lo);
                i += 2;
            } else {
                result += '%';
            }
        }
        else {
            result += c;
        }
    }
    return result;
}