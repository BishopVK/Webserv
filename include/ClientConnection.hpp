#ifndef CLIENTCONNECTION_HPP
#define CLIENTCONNECTION_HPP

#include "ServerConnection.hpp"
#include <string>

class ClientConnection
{
  private:
    // Petition related
    mutable long int     _requestSize;
    mutable bool    _headersRead;
    mutable long int  _contentLength;

    std::string       _read_buffer;
    std::string       _write_buffer;
    bool              _request_complete;
    bool              _response_sent;
    ServerConnection* _server_connection;

  public:
    ClientConnection();
    ClientConnection(const ClientConnection& other);
    ClientConnection& operator=(const ClientConnection& other);
    ~ClientConnection();

    // Getters
    const std::string& getReadBuffer() const;
    const std::string& getWriteBuffer() const;
    bool               isRequestComplete() const;
    bool               isResponseSent() const;
    ServerConnection*  getServerConnection() const;
    int                getRequestSize() const;
    bool               areHeadersRead() const;
    long int                getContentLength() const;

    // Setters
    void setReadBuffer(const std::string& buffer);
    void setWriteBuffer(const std::string& buffer);
    void setRequestComplete(bool complete);
    void setResponseSent(bool sent);
    void setServerConnection(ServerConnection* server_connection);

    // Buffer manipulation methods
    void appendToReadBuffer(const std::string& data);
    void appendToWriteBuffer(const std::string& data);
    void clearReadBuffer();
    void clearWriteBuffer();
    void eraseFromWriteBuffer(size_t pos, size_t len);

    // Utility methods
    bool hasCompleteRequest() const;
    bool hasServerConnection() const;
    void reset();
};

#endif
