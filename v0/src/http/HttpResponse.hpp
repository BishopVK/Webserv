#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <string>

class HttpResponse
{
  private:
    int         _statusCode;
    std::string _reasonPhrase;
    std::string _body;
    std::string _headers;

    void updateContentLength();

  public:
    HttpResponse();
    HttpResponse(const HttpResponse& other);
    HttpResponse& operator=(const HttpResponse& other);
    ~HttpResponse();

    void setStatus(int code, const std::string& reason);
    void setHeader(const std::string& key, const std::string& value);
    void setBody(const std::string& body);

    std::string toString() const;

    static HttpResponse ok(const std::string& body = "", const std::string& contentType = "text/plain");
    static HttpResponse badRequest(const std::string& body = "", const std::string& contentType = "text/plain");
    static HttpResponse notFound(const std::string& body = "", const std::string& contentType = "text/plain");
    static HttpResponse notImplemented(const std::string& body = "", const std::string& contentType = "text/plain");
    static HttpResponse methodNotAllowed(const std::string& body = "", const std::string& contentType = "text/plain");
};

#endif
