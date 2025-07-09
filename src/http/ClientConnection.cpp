#include "ClientConnection.hpp"

ClientConnection::ClientConnection() : _request_complete(false), _response_sent(false), _server_connection(NULL)
{
}

ClientConnection::ClientConnection(const ClientConnection& other)
    : _read_buffer(other._read_buffer), _write_buffer(other._write_buffer),
      _request_complete(other._request_complete), _response_sent(other._response_sent),
      _server_connection(other._server_connection)
{
}

ClientConnection& ClientConnection::operator=(const ClientConnection& other)
{
    if (this != &other)
    {
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

// Setters
void ClientConnection::setReadBuffer(const std::string& buffer)
{
    _read_buffer = buffer;
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
    _read_buffer += data;
}

void ClientConnection::appendToWriteBuffer(const std::string& data)
{
    _write_buffer += data;
}

void ClientConnection::clearReadBuffer()
{
    _read_buffer.clear();
}

void ClientConnection::clearWriteBuffer()
{
    _write_buffer.clear();
}

void ClientConnection::eraseFromWriteBuffer(size_t pos, size_t len)
{
    _write_buffer.erase(pos, len);
}

// Utility methods
bool ClientConnection::hasCompleteRequest() const
{
    return _read_buffer.find("\r\n\r\n") != std::string::npos;
}

bool ClientConnection::hasServerConnection() const
{
    return _server_connection != NULL;
}

void ClientConnection::reset()
{
    _read_buffer.clear();
    _write_buffer.clear();
    _request_complete = false;
    _response_sent = false;
    _server_connection = NULL;
}
