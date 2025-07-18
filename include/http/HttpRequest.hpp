#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <map>
#include <string>

class HttpRequest
{
  private:
    std::string                        _method;
    std::string                        _url;
    std::string                        _rawUrl;
    std::string                        _version;
    std::map<std::string, std::string> _headers;
    std::string                        _body;
    std::string                        _raw;
    std::map<std::string, std::string> _parameters;
    bool                               _valid;

    void parse(const char* raw_request);
    void parseUrlParameters(const std::string& url);

  public:
    HttpRequest();
    HttpRequest(const char* raw_request);
    HttpRequest(const HttpRequest& other);
    HttpRequest& operator=(const HttpRequest& other);
    ~HttpRequest();

    const std::string&                        getMethod() const;
    const std::string&                        getUrl() const;
    const std::string&                        getRawUrl() const;
    const std::string&                        getVersion() const;
    const std::map<std::string, std::string>& getHeaders() const;
    const std::string&                        getHeader(const std::string& key) const;
    const std::string&                        getBody() const;
    const std::string&                        getRaw() const;
    const std::map<std::string, std::string>& getParameters() const;
    const std::string&                        getParameter(const std::string& key) const;
    void                                      setBody(const std::string& body);
    const std::string                         getRawParameters() const;

    /**
     * Returns the boundary for multipart/form-data requests.
     */
    const std::string getBoundary() const;

    bool isValid() const;
};

#endif
