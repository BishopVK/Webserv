#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include <map>
#include <string>

class HttpResponse
{
  private:
    int                                _statusCode;
    std::string                        _reasonPhrase;
    std::string                        _body;
    std::map<std::string, std::string> _headers;

    void updateContentLength();

  public:
    HttpResponse();
    HttpResponse(const HttpResponse& other);
    HttpResponse& operator=(const HttpResponse& other);
    ~HttpResponse();

    void setStatus(int code, const std::string& reason);
    void setHeader(const std::string& key, const std::string& value);
    void setBody(const std::string& body);

    int                getStatusCode() const;
    const std::string& getReasonPhrase() const;

    std::string toString() const;

    // custom
    static HttpResponse response(int code, const std::string& reason, const std::string& body = "", const std::string& contentType = "text/plain");
    // Http 200
    static HttpResponse ok(const std::string& body = "", const std::string& contentType = "text/plain");
    // Http 400
    static HttpResponse badRequest(const std::string& body = "", const std::string& contentType = "text/plain");
    // Http 404
    static HttpResponse notFound(const std::string& body = "", const std::string& contentType = "text/plain");
    // Http 405
    static HttpResponse methodNotAllowed(const std::string& body = "", const std::string& contentType = "text/plain");
    // Http 413
    static HttpResponse requestEntityTooLarge(const std::string& message);
    // Http 500
    static HttpResponse internalServerError(const std::string& body = "", const std::string& contentType = "text/plain");
    // Http 501
    static HttpResponse notImplemented(const std::string& body = "", const std::string& contentType = "text/plain");
    // Http 504
    static HttpResponse gatewayTimeout(const std::string& body = "", const std::string& contentType = "text/plain");

    static HttpResponse redirect(const std::string& location, int code = 302);
};

#endif
