#include "FileSystemHandler.hpp"
#include "Logger.hpp"
#include <algorithm>
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>

FileSystemHandler::ResourceType FileSystemHandler::getResourceType(const std::string& path)
{
    struct stat pathStat;
    if (stat(path.c_str(), &pathStat) != 0)
        return NOT_FOUND;

    if (S_ISDIR(pathStat.st_mode))
        return DIRECTORY;
    else if (S_ISREG(pathStat.st_mode))
        return FILE;

    return NOT_FOUND;
}

std::string FileSystemHandler::getFileContent(const std::string& path)
{
    std::ifstream file(path.c_str());
    if (!file.is_open())
    {
        Logger::instance().error("Failed to open file: " + path);
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}

bool FileSystemHandler::fileExists(const std::string& path)
{
    struct stat pathStat;
    return stat(path.c_str(), &pathStat) == 0;
}

bool FileSystemHandler::isDirectory(const std::string& path)
{
    return getResourceType(path) == DIRECTORY;
}

bool FileSystemHandler::isFile(const std::string& path)
{
    return getResourceType(path) == FILE;
}

std::vector<std::string> FileSystemHandler::getDirectoryEntries(const std::string& path)
{
    std::vector<std::string> entries;
    DIR*                     dir = opendir(path.c_str());
    if (!dir)
        return entries;

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        std::string entryName = entry->d_name;
        if (entryName != "." && entryName != "..")
            entries.push_back(entryName);
    }
    closedir(dir);

    std::sort(entries.begin(), entries.end());
    return entries;
}

size_t FileSystemHandler::getFileSize(const std::string& path)
{
    struct stat pathStat;
    if (stat(path.c_str(), &pathStat) != 0)
        return 0;

    return pathStat.st_size;
}
