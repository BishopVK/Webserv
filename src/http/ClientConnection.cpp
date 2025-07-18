#include "ClientConnection.hpp"
#include <cstddef>
#include <cstdlib>
#include <string>

ClientConnection::ClientConnection()
    : _requestSize(0), _headersRead(false), _contentLength(-1), _request_complete(false), _response_sent(false), _server_connection(NULL)
{
}

ClientConnection::ClientConnection(const ClientConnection& other)
    : _requestSize(other._requestSize), _headersRead(other._headersRead), _contentLength(other._contentLength), _read_buffer(other._read_buffer),
      _write_buffer(other._write_buffer), _request_complete(other._request_complete), _response_sent(other._response_sent), _server_connection(other._server_connection)
{
}

ClientConnection& ClientConnection::operator=(const ClientConnection& other)
{
    if (this != &other)
    {
        _requestSize = other._requestSize;
        _headersRead = other._headersRead;
        _contentLength = other._contentLength;
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

int ClientConnection::getRequestSize() const
{
    return _requestSize;
}

bool ClientConnection::areHeadersRead() const
{
    return _headersRead;
}

long int ClientConnection::getContentLength() const
{
    return _contentLength;
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

bool ClientConnection::hasCompleteRequest() const
{
    size_t endHeaders = _read_buffer.find("\r\n\r\n");
    if (endHeaders == std::string::npos)
        return false;

    if (!_headersRead)
    {
        _headersRead = true;
        _contentLength = -1;

        size_t cl_pos = _read_buffer.find("Content-Length:");
        if (cl_pos == std::string::npos)
            cl_pos = _read_buffer.find("content-length:");

        if (cl_pos != std::string::npos && cl_pos < endHeaders)
        {
            size_t start = _read_buffer.find(":", cl_pos) + 1;
            size_t end = _read_buffer.find("\r\n", start);

            if (start != std::string::npos && end != std::string::npos)
            {
                std::string contentLength = _read_buffer.substr(start, end - start);

                size_t first = contentLength.find_first_not_of(" \t");
                if (first != std::string::npos)
                    _contentLength = std::atol(contentLength.c_str() + first);
            }
        }
    }

    if (_contentLength == -1 || _contentLength == 0)
        return true;

    size_t start = endHeaders + 4;
    size_t recived = _read_buffer.length() - start;

    return recived >= static_cast<size_t>(_contentLength);
}

bool ClientConnection::hasServerConnection() const
{
    return _server_connection != NULL;
}

void ClientConnection::reset()
{
    _requestSize = 0;
    _headersRead = false;
    _contentLength = -1;
    _read_buffer.clear();
    _write_buffer.clear();
    _request_complete = false;
    _response_sent = false;
    _server_connection = NULL;
}
