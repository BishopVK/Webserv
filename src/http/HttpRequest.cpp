#include "HttpRequest.hpp"
#include <sstream>
#include <string>

HttpRequest::HttpRequest(const char* raw_request) : _method(), _url(), _version(), _raw(raw_request ? raw_request : ""), _valid(false)
{
    parse(raw_request);
}

HttpRequest::HttpRequest(const HttpRequest& other)
    : _method(other._method), _url(other._url), _version(other._version), _raw(other._raw), _valid(other._valid)
{
}

HttpRequest& HttpRequest::operator=(const HttpRequest& other)
{
    if (this != &other)
    {
        _method = other._method;
        _url = other._url;
        _version = other._version;
        _raw = other._raw;
        _valid = other._valid;
    }
    return *this;
}

HttpRequest::~HttpRequest()
{
}

const std::string& HttpRequest::getMethod() const
{
    return _method;
}

const std::string& HttpRequest::getUrl() const
{
    return _url;
}

const std::string& HttpRequest::getVersion() const
{
    return _version;
}

const std::string& HttpRequest::getRaw() const
{
    return _raw;
}

const std::map<std::string, std::string>& HttpRequest::getHeaders() const
{
    return _headers;
}

const std::string& HttpRequest::getHeader(const std::string& key) const
{
    std::map<std::string, std::string>::const_iterator it = _headers.find(key);
    if (it != _headers.end())
        return it->second;

    static const std::string emptyString("");
    return emptyString;
}

const std::string& HttpRequest::getBody() const
{
    return _body;
}

bool HttpRequest::isValid() const
{
    return _valid;
}

void HttpRequest::parse(const char* raw_request)
{
    _valid = false;
    if (!raw_request)
        return;

    _raw = raw_request;
    std::istringstream iss(_raw);
    std::string        line;

    // 1. Se recupera la primera línea y se valida su existencia
    if (!std::getline(iss, line) || line.empty())
        return;

    // 2. Separa la primera línea en metodo URL y version
    std::istringstream line_stream(line);
    if (!(line_stream >> _method >> _url >> _version))
        return;

    // 3. Se comprueba que el metodo sea valido, por lo que si es correcto method y url no deben estar vacios
    if (_version.find("HTTP/1.") != 0)
        return;

    // 4. Se recuperan los headers
    _headers.clear();
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
                _headers[key] = value;
        }
    }

    // 5. Se recupera el body
    std::ostringstream body_stream;
    body_stream << iss.rdbuf();
    _body = body_stream.str();

    _valid = true;
}
