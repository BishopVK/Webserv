#include "HttpResponse.hpp"
#include <cstring>
#include <sstream>

HttpResponse::HttpResponse() : _statusCode(200), _reasonPhrase("OK"), _body(""), _headers("")
{
    setHeader("Content-Type", "text/plain");
    updateContentLength();
}

HttpResponse::HttpResponse(const HttpResponse& other)
    : _statusCode(other._statusCode), _reasonPhrase(other._reasonPhrase), _body(other._body), _headers(other._headers)
{
}

HttpResponse& HttpResponse::operator=(const HttpResponse& other)
{
    if (this != &other)
    {
        _statusCode = other._statusCode;
        _reasonPhrase = other._reasonPhrase;
        _body = other._body;
        _headers = other._headers;
    }
    return *this;
}

HttpResponse::~HttpResponse()
{
}

void HttpResponse::setStatus(int code, const std::string& reason)
{
    _statusCode = code;
    _reasonPhrase = reason;
}

void HttpResponse::setHeader(const std::string& key, const std::string& value)
{
    _headers += key + ": " + value + "\r\n";
}

void HttpResponse::setBody(const std::string& body)
{
    _body = body;
    updateContentLength();
}

void HttpResponse::updateContentLength()
{
    size_t pos = _headers.find("Content-Length:");
    if (pos != std::string::npos)
    {
        size_t end = _headers.find("\r\n", pos);
        if (end != std::string::npos)
            _headers.erase(pos, end - pos + 2);
        else
            _headers.erase(pos);
    }
    std::ostringstream oss;
    oss << "Content-Length: " << _body.size() << "\r\n";
    _headers += oss.str();
}

std::string HttpResponse::toString() const
{
    std::ostringstream oss;
    oss << "HTTP/1.1 " << _statusCode << " " << _reasonPhrase << "\r\n";
    oss << _headers;
    oss << "\r\n";
    oss << _body;
    return oss.str();
}

HttpResponse HttpResponse::ok(const std::string& body, const std::string& contentType)
{
    HttpResponse response;
    response.setStatus(200, "OK");
    response.setHeader("Content-Type", contentType);
    response.setBody(body);
    return response;
}

HttpResponse HttpResponse::badRequest(const std::string& body, const std::string& contentType)
{
    HttpResponse response;
    response.setStatus(400, "Bad Request");
    response.setHeader("Content-Type", contentType);
    response.setBody(body);
    return response;
}

HttpResponse HttpResponse::notFound(const std::string& body, const std::string& contentType)
{
    HttpResponse response;
    response.setStatus(404, "Not Found");
    response.setHeader("Content-Type", contentType);
    response.setBody(body);
    return response;
}

HttpResponse HttpResponse::notImplemented(const std::string& body, const std::string& contentType)
{
    HttpResponse response;
    response.setStatus(501, "Not Implemented");
    response.setHeader("Content-Type", contentType);
    response.setBody(body);
    return response;
}

HttpResponse HttpResponse::methodNotAllowed(const std::string& body, const std::string& contentType)
{
    HttpResponse response;
    response.setStatus(405, "Method Not Allowed");
    response.setHeader("Content-Type", contentType);
    response.setBody(body);
    return response;
}
