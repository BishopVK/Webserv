#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <map>
#include <string>

class HttpRequest
{
  private:
    std::string                        _method;
    std::string                        _url;
    std::string                        _version;
    std::map<std::string, std::string> _headers;
    std::string                        _body;
    std::string                        _raw;
    bool                               _valid;

    void parse(const char* raw_request);

  public:
    HttpRequest(const char* raw_request);
    HttpRequest(const HttpRequest& other);
    HttpRequest& operator=(const HttpRequest& other);
    ~HttpRequest();

    const std::string&                        getMethod() const;
    const std::string&                        getUrl() const;
    const std::string&                        getVersion() const;
    const std::map<std::string, std::string>& getHeaders() const;
    const std::string&                        getHeader(const std::string& key) const;
    const std::string&                        getBody() const;
    const std::string&                        getRaw() const;

    bool isValid() const;
};

#endif
