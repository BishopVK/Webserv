#ifndef CGI_HANDLER_HPP
#define CGI_HANDLER_HPP

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include <string>

class CgiHandler
{
  private:
    static std::string getCgiFilePath(const HttpRequest& request, const Location* location, const Server* server);
    static std::string getDefaultCgiFile(const Location* location, const Server* server);
    static bool        hasValidCgiExtension(const std::string& filePath, const std::string& cgiExtension);

  public:
    CgiHandler();
    CgiHandler(const CgiHandler& other);
    CgiHandler& operator=(const CgiHandler& other);
    ~CgiHandler();

    static bool         isCgiRequest(const std::string& requestPath, const Location* location);
    static bool         isCgiLocation(const Location* location);
    static HttpResponse execute(const HttpRequest& request, const Location* location, const Server* server);
};

#endif
