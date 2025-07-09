#ifndef HTTP_REQUEST_HANDLER_HPP
#define HTTP_REQUEST_HANDLER_HPP

#include "ClientConnection.hpp"
#include <map>
#include <string>

class HttpRequest;
class HttpResponse;
class BadRequestHttpResponse;

class HttpRequestHandler
{
  private:
    static std::map<std::string, std::string> _contentTypeMap;
    std::string                               generateAutoIndexHtml(const std::string& path) const;
    std::string                               getFileContent(const std::string& path) const;
    std::string                               getContentType(const std::string& path) const;

    enum ResourceType
    {
        FILE,
        DIRECTORY,
        NOT_FOUND
    };

    ResourceType getResourceType(const std::string& path) const;

  public:
    HttpRequestHandler();
    HttpRequestHandler(const HttpRequestHandler& other);
    HttpRequestHandler& operator=(const HttpRequestHandler& other);
    ~HttpRequestHandler();

    HttpResponse handle(const HttpRequest& request, const ClientConnection& client) const;
};

#endif
