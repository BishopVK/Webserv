#include "ClientConnection.hpp"
#include "Logger.hpp"
#include <cstddef>
#include <cstdlib>
#include <string>

ClientConnection::ClientConnection()
    : _requestSize(0), _headersRead(false), _contentLength(static_cast<size_t>(-1)), _requestParsed(false), _httpRequest(NULL), _request_complete(false), _response_sent(false), _server_connection(NULL)
{
}

ClientConnection::ClientConnection(const ClientConnection& other)
    : _requestSize(other._requestSize), _headersRead(other._headersRead), _contentLength(other._contentLength), _requestParsed(other._requestParsed), _httpRequest(NULL),
      _read_buffer(other._read_buffer), _write_buffer(other._write_buffer), _request_complete(other._request_complete), _response_sent(other._response_sent),
      _server_connection(other._server_connection)
{
    if (other._httpRequest)
        _httpRequest = new HttpRequest(*other._httpRequest);
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
    // 1. Check for end of headers
    size_t endHeadersPos = _read_buffer.find("\r\n\r\n");
    if (endHeadersPos == std::string::npos)
        return false;

    // 2. Now that headers are complete check if we have alredy parsed the partial request
    if (!_requestParsed)
    {
        // 3. Parse the headers
        std::string headerPortion = _read_buffer.substr(0, endHeadersPos + 4);
        _httpRequest = new HttpRequest(headerPortion.c_str());
        if (!_httpRequest->isValid())
        {
            Logger::instance().error("Invalid HTTP request received.");
            delete _httpRequest;
            _httpRequest = NULL;
            return false; // Invalid request
        }
        _requestParsed = true;
        _headSize = endHeadersPos + 4; // Store the size of the headers

        // 4. Now that we have parsed the headers, get the content length and if a boundary is present
        std::string contentLengthStr = _httpRequest->getHeader("Content-Length");
        if (!contentLengthStr.empty())
        {
            try
            {
                // _contentLength = std::stoul(contentLengthStr);
                // c++ 98
                _contentLength = static_cast<size_t>(std::strtoul(contentLengthStr.c_str(), NULL, 10));
            }
            catch (const std::exception& e)
            {
                Logger::instance().error("Invalid Content-Length header: " + contentLengthStr);
                _contentLength = static_cast<size_t>(-1);
            }
        }
        else
            _contentLength = 0;

        // 5. Check for multipart requests
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
                return false;
            }
        }
    }

    // 6. If we have a content length, check if we have read enough data
    bool isContentLengthSurpassed = (_contentLength != static_cast<size_t>(-1) && _read_buffer.length() >= _headSize + _contentLength);

    return isContentLengthSurpassed;
}

bool ClientConnection::hasServerConnection() const
{
    return _server_connection != NULL;
}

void ClientConnection::reset()
{
    _requestSize = 0;
    _headersRead = false;
    _contentLength = static_cast<size_t>(-1);
    _requestParsed = false;
    if (_httpRequest)
    {
        delete _httpRequest;
        _httpRequest = NULL;
    }
    _read_buffer.clear();
    _write_buffer.clear();
    _request_complete = false;
    _response_sent = false;
    _server_connection = NULL;
}
