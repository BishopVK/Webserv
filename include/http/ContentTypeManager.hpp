#ifndef CONTENT_TYPE_MANAGER_HPP
#define CONTENT_TYPE_MANAGER_HPP

#include <map>
#include <string>

class ContentTypeManager
{
  private:
    static std::map<std::string, std::string> _contentTypeMap;
    static bool                               _initialized;

    static void initialize();

  public:
    static std::string getContentType(const std::string& filePath);
};

#endif
