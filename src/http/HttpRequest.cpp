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

    raw = raw_request;
    std::istringstream iss(raw);
    std::string        line;

    // 1. Se recupera la primera línea y se valida su existencia
    if (!std::getline(iss, line) || line.empty())
        return;

    // 2. Separa la primera línea en metodo URL y version
    std::istringstream line_stream(line);
    if (!(line_stream >> method >> url >> version))
        return;

    // 3. Se comprueba que el metodo sea valido, por lo que si es correcto method y url no deben estar vacios
    if (version.find("HTTP/1.") != 0)
        return;

    // 4. Se recuperan los headers
    headers.clear();
    while (std::getline(iss, line))
    {
        if (line == "\r" || line.empty()) // Indica el final de los headers
            break;
        size_t pos = line.find(':');
        if (pos != std::string::npos)
        {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            while (!value.empty() && (value[0] == ' ' || value[0] == '\t'))
                value.erase(0, 1);
            if (!key.empty() && !value.empty())
                headers[key] = value;
        }
    }

    // 5. Se recupera el body
    std::ostringstream body_stream;
    body_stream << iss.rdbuf();
    body = body_stream.str();

    valid = true;
}
