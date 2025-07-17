#ifndef FILE_SYSTEM_HANDLER_HPP
#define FILE_SYSTEM_HANDLER_HPP

#include <string>
#include <vector>

class FileSystemHandler
{
  private:
    FileSystemHandler();
    FileSystemHandler(const FileSystemHandler& other);
    FileSystemHandler& operator=(const FileSystemHandler& other);
    ~FileSystemHandler();

  public:
    enum ResourceType
    {
        FILE,
        DIRECTORY,
        NOT_FOUND
    };

    static ResourceType             getResourceType(const std::string& path);
    static std::string              getFileContent(const std::string& path);
    static bool                     fileExists(const std::string& path);
    static bool                     directoryExists(const std::string& path);
    static bool                     isDirectory(const std::string& path);
    static bool                     isFile(const std::string& path);
    static std::vector<std::string> getDirectoryEntries(const std::string& path);

    static bool deleteFile(const std::string& path);
    static bool deleteDirectory(const std::string& path);

    static size_t getFileSize(const std::string& path);
};

#endif
