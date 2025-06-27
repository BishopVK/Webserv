#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>

class HttpRequest
{
  private:
    std::string method;
    std::string url;
    std::string version;
    std::string raw;
    bool        valid;

    void parse(const char* raw_request);

  public:
    HttpRequest(const char* raw_request);
    HttpRequest(const HttpRequest& other);
    HttpRequest& operator=(const HttpRequest& other);
    ~HttpRequest();

    const std::string& getMethod() const;
    const std::string& getUrl() const;
    const std::string& getVersion() const;
    const std::string& getRaw() const;

    bool isValid() const;
};

#endif
