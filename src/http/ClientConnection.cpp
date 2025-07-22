#include "ClientConnection.hpp"
#include "Logger.hpp"
#include <cstddef>
#include <cstdlib>
#include <string>

ClientConnection::ClientConnection()
    : _requestSize(0), _headersRead(false), _contentLength(static_cast<size_t>(-1)), _headSize(0), _requestParsed(false), _httpRequest(NULL), _isMultipart(false), _boundary(""), _read_buffer(""), _write_buffer(""), _request_complete(false), _response_sent(false), _server_connection(NULL)
{
}

ClientConnection::ClientConnection(const ClientConnection& other)
    : _requestSize(other._requestSize), _headersRead(other._headersRead), _contentLength(other._contentLength), _headSize(other._headSize),
      _requestParsed(other._requestParsed), _isMultipart(other._isMultipart), _boundary(other._boundary), _read_buffer(other._read_buffer),
      _write_buffer(other._write_buffer), _request_complete(other._request_complete), _response_sent(other._response_sent), _server_connection(other._server_connection)
{
    if (other._httpRequest)
        _httpRequest = new HttpRequest(*other._httpRequest);
    else
        _httpRequest = NULL;
}

ClientConnection& ClientConnection::operator=(const ClientConnection& other)
{
    if (this != &other)
    {
        _requestSize = other._requestSize;
        _headersRead = other._headersRead;
        _contentLength = other._contentLength;
        _requestParsed = other._requestParsed;
        delete _httpRequest;
        _httpRequest = NULL;
        if (other._httpRequest)
            _httpRequest = new HttpRequest(*other._httpRequest);
        _read_buffer = other._read_buffer;
        _write_buffer = other._write_buffer;
        _request_complete = other._request_complete;
        _response_sent = other._response_sent;
        _server_connection = other._server_connection;
        _isMultipart = other._isMultipart;
        _boundary = other._boundary;
        _headSize = other._headSize;
    }
    return *this;
}

ClientConnection::~ClientConnection()
{
    delete _httpRequest;
}

// Getters
const std::string& ClientConnection::getReadBuffer() const
{
    return _read_buffer;
}

const std::string& ClientConnection::getWriteBuffer() const
{
    return _write_buffer;
}

bool ClientConnection::isRequestComplete() const
{
    return _request_complete;
}

bool ClientConnection::isResponseSent() const
{
    return _response_sent;
}

ServerConnection* ClientConnection::getServerConnection() const
{
    return _server_connection;
}

size_t ClientConnection::getRequestSize() const
{
    return _requestSize;
}

bool ClientConnection::areHeadersRead() const
{
    return _headersRead;
}

size_t ClientConnection::getContentLength() const
{
    return _contentLength;
}

HttpRequest* ClientConnection::getHttpRequest() const
{
    return _httpRequest;
}

bool ClientConnection::isRequestParsed() const
{
    return _requestParsed;
}

// Setters
void ClientConnection::setReadBuffer(const std::string& buffer)
{
    _read_buffer = buffer;
    _requestSize = buffer.length();
}

void ClientConnection::setWriteBuffer(const std::string& buffer)
{
    _write_buffer = buffer;
}

void ClientConnection::setRequestComplete(bool complete)
{
    _request_complete = complete;
}

void ClientConnection::setResponseSent(bool sent)
{
    _response_sent = sent;
}

void ClientConnection::setServerConnection(ServerConnection* server_connection)
{
    _server_connection = server_connection;
}

void ClientConnection::appendToReadBuffer(const std::string& data)
{
    _read_buffer.append(data);
    _requestSize += data.length();
}

void ClientConnection::appendToWriteBuffer(const std::string& data)
{
    _write_buffer += data;
}

void ClientConnection::clearReadBuffer()
{
    _read_buffer.clear();
    _requestSize = 0;
}

void ClientConnection::clearWriteBuffer()
{
    _write_buffer.clear();
}

void ClientConnection::eraseFromWriteBuffer(size_t pos, size_t len)
{
    _write_buffer.erase(pos, len);
}

bool ClientConnection::hasCompleteRequest()
{
    size_t endHeadersPos = _read_buffer.find("\r\n\r\n");
    if (endHeadersPos == std::string::npos)
        return false;

    if (!_requestParsed)
    {
        std::string headerPortion = _read_buffer.substr(0, endHeadersPos + 4);
        delete _httpRequest;
        _httpRequest = new HttpRequest(headerPortion.c_str());

        if (!_httpRequest->isValid())
        {
            Logger::instance().error("Invalid HTTP request received.");
            delete _httpRequest;
            _httpRequest = NULL;
            return false;
        }

        _requestParsed = true;
        _headSize = endHeadersPos + 4;

        std::string contentLengthStr = _httpRequest->getHeader("Content-Length");
        if (!contentLengthStr.empty())
        {
            try
            {
                _contentLength = static_cast<size_t>(std::strtoul(contentLengthStr.c_str(), NULL, 10));
            }
            catch (const std::exception& e)
            {
                Logger::instance().error("Invalid Content-Length header: " + contentLengthStr);
                _contentLength = 0;
            }
        }
        else
            _contentLength = 0;

        std::string contentType = _httpRequest->getHeader("Content-Type");
        if (contentType.find("multipart/form-data") != std::string::npos)
        {
            _isMultipart = true;
            _boundary = _httpRequest->getBoundary();
            if (_boundary.empty())
            {
                Logger::instance().error("Multipart request without boundary.");
                delete _httpRequest;
                _httpRequest = NULL;
                _requestParsed = false;
                return false;
            }
        }
        else
            _isMultipart = false;
    }

    return _isMultipart ? isMultipartComplete() : isRegularRequestComplete();
}

std::string ClientConnection::uriDecode(const std::string &src)
{
    std::string dest;
    for (size_t i = 0; i < src.length(); ++i) {
        if (src[i] == '%') {
            // Convert the next two characters from hex to decimal
            if (i + 2 < src.length()) {
                std::string hex = src.substr(i + 1, 2);
                int value = 0;
                std::istringstream(hex) >> std::hex >> value;
                dest += static_cast<char>(value);
                i += 2; // Skip the next two characters
            }
        } else if (src[i] == '+') {
            // Convert '+' to space
            dest += ' ';
        } else {
            dest += src[i];
        }
    }
    return dest;
}

std::string ClientConnection::uriEncode(const std::string &src)
{
    std::ostringstream encoded;
    for (size_t i = 0; i < src.length(); ++i) {
        unsigned char c = src[i];
        // Si el carácter es alfanumérico o uno de los caracteres permitidos, lo agregamos directamente
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded << c;
        } else {
            // De lo contrario, lo convertimos a percent-encoding
            encoded << '%' << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(c);
        }
    }
    return encoded.str();
}

bool ClientConnection::isMultipartComplete()
{
    if (_boundary.empty())
        return false;

    std::string finalBoundary = "--" + _boundary + "--";

    size_t finalBoundaryPos = _read_buffer.find(finalBoundary);
    if (finalBoundaryPos == std::string::npos)
        return false;

    size_t afterBoundary = finalBoundaryPos + finalBoundary.length();

    while (afterBoundary < _read_buffer.length() && (_read_buffer[afterBoundary] == ' ' || _read_buffer[afterBoundary] == '\t'))
        afterBoundary++;

    bool properlyTerminated = false;
    if (afterBoundary >= _read_buffer.length())
        properlyTerminated = true;
    else if (afterBoundary + 1 < _read_buffer.length() && _read_buffer[afterBoundary] == '\r' && _read_buffer[afterBoundary + 1] == '\n')
        properlyTerminated = true;
    else if (_read_buffer[afterBoundary] == '\n')
        properlyTerminated = true;

    if (properlyTerminated)
        return true;

    return false;
}

bool ClientConnection::isRegularRequestComplete()
{
    if (_contentLength == 0)
        return true;

    if (_contentLength == static_cast<size_t>(-1))
        return true;

    size_t expectedTotal = _headSize + _contentLength;
    return _read_buffer.length() >= expectedTotal;
}

bool ClientConnection::hasServerConnection() const
{
    return _server_connection != NULL;
}
