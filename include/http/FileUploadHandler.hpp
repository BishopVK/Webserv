#ifndef FILE_UPLOAD_HANDLER_HPP
#define FILE_UPLOAD_HANDLER_HPP

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include <string>

class FileUploadHandler
{
  private:
    static bool saveFile(const std::string& filePath, const std::string& content);
    static bool isValidUploadPath(const std::string& path);

    FileUploadHandler();
    FileUploadHandler(const FileUploadHandler& other);
    FileUploadHandler& operator=(const FileUploadHandler& other);
    ~FileUploadHandler();

  public:
    static bool         isUploadRequest(const HttpRequest& request, const Location* location);
    static HttpResponse handleUpload(const HttpRequest& request, const Location* location, const Server* server);
};

#endif
