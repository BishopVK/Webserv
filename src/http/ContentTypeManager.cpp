#include "ContentTypeManager.hpp"
#include <algorithm>

std::map<std::string, std::string> ContentTypeManager::_contentTypeMap;
bool                               ContentTypeManager::_initialized = false;

void ContentTypeManager::initialize()
{
    if (_initialized)
        return;

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
    _contentTypeMap["mp4"] = "video/mp4";
    _contentTypeMap["mp3"] = "audio/mpeg";

    _initialized = true;
}

std::string ContentTypeManager::getContentType(const std::string& filePath)
{
    initialize();

    size_t dotPos = filePath.find_last_of('.');
    if (dotPos == std::string::npos)
        return "text/plain";

    std::string extension = filePath.substr(dotPos + 1);

    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    std::map<std::string, std::string>::const_iterator it = _contentTypeMap.find(extension);
    if (it != _contentTypeMap.end())
        return it->second;

    return "text/plain";
}
