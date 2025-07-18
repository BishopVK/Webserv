#include "../../include/http/HttpResponse.hpp"
#include <iostream>
#include <sstream>

HttpResponse::HttpResponse() : _statusCode(200), _reasonPhrase("OK"), _body(""), _headers()
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
    _headers[key] = value;
}

void HttpResponse::setBody(const std::string& body)
{
    _body = body;
    updateContentLength();
}

void HttpResponse::updateContentLength()
{
    std::ostringstream oss;
    oss << _body.size();
    _headers["Content-Length"] = oss.str();
}

std::string HttpResponse::toString() const
{
    std::ostringstream oss;
    oss << "HTTP/1.1 " << _statusCode << " " << _reasonPhrase << "\r\n";

    for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it)
    {
        oss << it->first << ": " << it->second << "\r\n";
    }

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

HttpResponse HttpResponse::internalServerError(const std::string& body, const std::string& contentType)
{
    HttpResponse response;
    response.setStatus(500, "Internal Server Error");
    response.setHeader("Content-Type", contentType);
    response.setBody(body);
    return response;
}

HttpResponse HttpResponse::response(int code, const std::string& reason, const std::string& body, const std::string& contentType)
{
    HttpResponse response;
    response.setStatus(code, reason);
    response.setHeader("Content-Type", contentType);
    response.setBody(body);
    return response;
}

HttpResponse HttpResponse::redirect(const std::string& location, int code)
{
    HttpResponse response;
    response.setStatus(code, "Redirect");
    response.setHeader("Location", location);
    return response;
}
