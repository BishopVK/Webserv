#ifndef CLIENTCONNECTION_HPP
#define CLIENTCONNECTION_HPP

#include "ServerConnection.hpp"
#include "http/HttpRequest.hpp"
#include <string>

class ClientConnection
{
  private:
    size_t       _requestSize;
    bool         _headersRead;
    size_t       _contentLength;
    size_t       _headSize;
    bool         _requestParsed;
    HttpRequest* _httpRequest;
    bool         _isMultipart;
    std::string  _boundary;

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
    size_t             getRequestSize() const;
    bool               areHeadersRead() const;
    size_t             getContentLength() const;
    HttpRequest*       getHttpRequest() const;
    bool               isRequestParsed() const;

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
    bool hasCompleteRequest();
    bool hasServerConnection() const;

    std::string uriDecode(const std::string &src);
    std::string uriEncode(const std::string &src);

    // Helper methods
    bool isMultipartComplete();
    bool isRegularRequestComplete();
};

#endif
