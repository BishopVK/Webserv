#include "FileSystemHandler.hpp"
#include "Logger.hpp"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

FileSystemHandler::FileSystemHandler()
{
}

FileSystemHandler::FileSystemHandler(const FileSystemHandler& other)
{
    (void)other;
}

FileSystemHandler& FileSystemHandler::operator=(const FileSystemHandler& other)
{
    (void)other;
    return *this;
}

FileSystemHandler::~FileSystemHandler()
{
}

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

bool FileSystemHandler::directoryExists(const std::string& path)
{
    struct stat pathStat;
    if (stat(path.c_str(), &pathStat) != 0)
        return false;

    return S_ISDIR(pathStat.st_mode);
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

std::vector<FileSystemHandler::DirectoryEntry> FileSystemHandler::getDirectoryEntriesWithInfo(const std::string& path)
{
    std::vector<DirectoryEntry> entries;
    DIR*                        dir = opendir(path.c_str());
    if (!dir)
        return entries;

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        std::string entryName = entry->d_name;
        if (entryName == "." || entryName == "..")
            continue;

        DirectoryEntry dirEntry;
        dirEntry.name = entryName;
        std::string fullPath = path + "/" + entryName;
        dirEntry.type = getResourceType(fullPath);
        dirEntry.size = 0;

        if (dirEntry.type == FILE)
            dirEntry.size = getFileSize(fullPath);

        entries.push_back(dirEntry);
    }
    closedir(dir);

    return entries;
}

size_t FileSystemHandler::getFileSize(const std::string& path)
{
    struct stat pathStat;
    if (stat(path.c_str(), &pathStat) != 0)
        return 0;

    return pathStat.st_size;
}

bool FileSystemHandler::deleteFile(const std::string& path)
{
    if (!fileExists(path))
    {
        Logger::instance().error("File does not exist: " + path);
        return false;
    }

    if (isDirectory(path))
    {
        Logger::instance().error("Cannot delete a directory with deleteFile: " + path);
        return false;
    }

    if (std::remove(path.c_str()) != 0)
    {
        Logger::instance().error("Failed to delete file: " + path);
        return false;
    }

    return true;
}

bool FileSystemHandler::writeFile(const std::string& path, const std::string& content)
{
    std::ofstream file(path.c_str(), std::ios::binary);
    if (!file.is_open())
    {
        Logger::instance().error("Failed to open file for writing: " + path);
        return false;
    }

    file.write(content.data(), content.size());

    if (file.fail())
    {
        Logger::instance().error("Failed to write file content: " + path);
        file.close();
        return false;
    }

    file.close();
    return true;
}
