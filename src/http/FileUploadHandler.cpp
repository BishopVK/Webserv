#include "FileUploadHandler.hpp"
#include "ErrorPageGenerator.hpp"
#include "FileSystemHandler.hpp"
#include "IntValue.hpp"
#include "Logger.hpp"
#include "PathHandler.hpp"
#include <sstream>
#include <string>

FileUploadHandler::FileUploadHandler()
{
}

FileUploadHandler::FileUploadHandler(const FileUploadHandler& other)
{
    (void)other;
}

FileUploadHandler& FileUploadHandler::operator=(const FileUploadHandler& other)
{
    (void)other;
    return *this;
}

FileUploadHandler::~FileUploadHandler()
{
}

bool FileUploadHandler::isUploadRequest(const HttpRequest& request, const Location* location)
{
    if (!location || !location->getUploadEnable())
        return false;

    std::string contentType = request.getHeader("Content-Type");
    if (contentType.empty())
        return false;

    return contentType.find("multipart/form-data") != std::string::npos;
}

HttpResponse FileUploadHandler::handleUpload(const HttpRequest& request, const Location* location, const Server* server)
{
    if (!location || !location->getUploadEnable())
        return ErrorPageGenerator::GenerateErrorResponse(HttpResponse::methodNotAllowed());

    std::string uploadStore = location->getUploadStore();
    if (uploadStore.empty())
    {
        Logger::instance().error("Upload store not configured for location");
        return ErrorPageGenerator::GenerateErrorResponse(HttpResponse::internalServerError());
    }

    std::string documentRoot = location->getRoot();
    if (documentRoot.empty())
        documentRoot = server->getRoot();

    std::string uploadDir = PathHandler::joinFilePath(documentRoot, uploadStore);

    if (!FileSystemHandler::directoryExists(uploadDir))
    {
        // TODO: Revisar si se deberia de crear el directorio en caso de que no exista
        Logger::instance().error("Upload directory does not exist: " + uploadDir);
        return ErrorPageGenerator::GenerateErrorResponse(HttpResponse::internalServerError());
    }

    // TODO: Esto puede cambiar quizas deba abstraerlo
    std::string contentType = request.getHeader("Content-Type");
    if (contentType.find("multipart/form-data") == std::string::npos)
    {
        Logger::instance().error("Only multipart/form-data uploads are supported");
        return ErrorPageGenerator::GenerateErrorResponse(HttpResponse::badRequest());
    }

    std::vector<File> files = request.getMultipartFiles();
    if (files.empty())
    {
        Logger::instance().error("No files found in multipart request");
        return ErrorPageGenerator::GenerateErrorResponse(HttpResponse::badRequest());
    }

    std::ostringstream responseStream;
    responseStream << "Files uploaded successfully:\n";
    int uploadCount = 0;

    for (std::vector<File>::const_iterator it = files.begin(); it != files.end(); ++it)
    {
        const File& file = *it;

        if (file.getName().empty() && file.getContent().empty())
            continue;

        std::string filename = file.getName();
        std::string fileContent = file.getContent();

        if (fileContent.empty())
            continue;

        if (filename.empty())
            filename = "file";

        std::string filePath = PathHandler::joinFilePath(uploadDir, filename);

        if (FileSystemHandler::fileExists(filePath))
        {
            Logger::instance().error("File already exists: " + filePath);
            continue;
        }

        if (!isValidUploadPath(filePath))
        {
            Logger::instance().error("Invalid upload path: " + filePath);
            continue;
        }

        if (!saveFile(filePath, fileContent))
        {
            Logger::instance().error("Failed to save uploaded file: " + filePath);
            continue;
        }

        int         fileSize = fileContent.size() > 1024 ? fileContent.size() / 1024 : fileContent.size();
        std::string sizeStr = fileContent.size() > 1024 ? IntValue(fileSize).toString() + " KB" : IntValue(fileSize).toString() + " bytes";

        Logger::instance().info("File uploaded successfully: " + filename + " (" + sizeStr + ")");

        responseStream << "- " << filename << " (" << sizeStr << ")\n";
        uploadCount++;
    }

    if (uploadCount == 0)
    {
        Logger::instance().error("No valid files were uploaded");
        return ErrorPageGenerator::GenerateErrorResponse(HttpResponse::badRequest());
    }

    return HttpResponse::response(201, "Created", responseStream.str(), "text/plain");
}

bool FileUploadHandler::saveFile(const std::string& filePath, const std::string& content)
{
    return FileSystemHandler::writeFile(filePath, content);
}

bool FileUploadHandler::isValidUploadPath(const std::string& path)
{
    // Basic path validation to prevent directory traversal
    if (path.find("..") != std::string::npos)
        return false;

    if (path.empty())
        return false;

    // Check for null bytes
    if (path.find('\0') != std::string::npos)
        return false;

    return true;
}
