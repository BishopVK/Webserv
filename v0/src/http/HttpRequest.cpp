#include "HttpRequest.hpp"
#include <sstream>

HttpRequest::HttpRequest(const char* raw_request)
    : method(), url(), version(), raw(raw_request ? raw_request : ""), valid(false)
{
    parse(raw_request);
}

HttpRequest::HttpRequest(const HttpRequest& other)
    : method(other.method), url(other.url), version(other.version), raw(other.raw), valid(other.valid)
{
}

HttpRequest& HttpRequest::operator=(const HttpRequest& other)
{
    if (this != &other)
    {
        method = other.method;
        url = other.url;
        version = other.version;
        raw = other.raw;
        valid = other.valid;
    }
    return *this;
}

HttpRequest::~HttpRequest()
{
}

const std::string& HttpRequest::getMethod() const
{
    return method;
}

const std::string& HttpRequest::getUrl() const
{
    return url;
}

const std::string& HttpRequest::getVersion() const
{
    return version;
}

const std::string& HttpRequest::getRaw() const
{
    return raw;
}

bool HttpRequest::isValid() const
{
    return valid;
}

void HttpRequest::parse(const char* raw_request)
{
    valid = false;
    if (!raw_request)
        return;

    std::istringstream iss(raw_request);
    std::string        line;

    if (!std::getline(iss, line))
        return;

    std::istringstream line_stream(line);

    if (!(line_stream >> method))
        return;

    if (!(line_stream >> url))
        return;

    if (!(line_stream >> version))
        return;

    if (version.find("HTTP/1.") != 0)
        return;

    valid = true;
}
