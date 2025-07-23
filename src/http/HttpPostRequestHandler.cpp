#include "HttpPostRequestHandler.hpp"
#include "CgiHandler.hpp"
#include "ErrorPageGenerator.hpp"
#include "FileUploadHandler.hpp"
#include "HttpResponse.hpp"

HttpPostRequestHandler::HttpPostRequestHandler() : AbstractHttpMethodHandler()
{
}

HttpPostRequestHandler::HttpPostRequestHandler(const HttpPostRequestHandler& other) : AbstractHttpMethodHandler(other)
{
}

HttpPostRequestHandler& HttpPostRequestHandler::operator=(const HttpPostRequestHandler& other)
{
    (void)other;
    return *this;
}

HttpPostRequestHandler::~HttpPostRequestHandler()
{
}

// HttpResponse HttpPostRequestHandler::handleMethod(const HttpRequest& request, const Location* location, const Server* server) const
// {
//     if (location && !isMethodAllowed("POST", *location))
//         return ErrorPageGenerator::GenerateErrorResponse(HttpResponse::methodNotAllowed());

//     std::string requestPath = request.getUrl();
//     if (requestPath.empty())
//         requestPath = "/";

//     // Manejo CGI
//     if (CgiHandler::isCgiRequest(requestPath, location))
//         return CgiHandler::execute(request, location, server);

//     std::string root = location ? location->getRoot() : server->getRoot();
//     std::string locPath = location ? location->getRoute() : "/";
//     std::string relativePath = requestPath;

//     if (requestPath.compare(0, locPath.size(), locPath) == 0)
//         relativePath = requestPath.substr(locPath.size());

//     // Construcción manual de la ruta completa:
//     std::string fullPath = root;
//     if (fullPath[fullPath.size() - 1] != '/' && relativePath[0] != '/')
//         fullPath += "/";
//     fullPath += relativePath;

//     if (fullPath[fullPath.size() - 1] != '/')
//         fullPath += "/";

//     // Nombre archivo con timestamp
//     std::stringstream ss;
//     ss << "message_" << std::time(NULL) << ".txt";
//     std::string filename = ss.str();

//     std::string fullFilePath = fullPath + filename;

//     std::cout << "Intentando escribir en: " << fullFilePath << std::endl;

//     std::ofstream out(fullFilePath.c_str(), std::ios::binary);
//     if (!out.is_open())
//     {
//         std::cerr << "ERROR: No se pudo abrir el archivo: " << fullFilePath << std::endl;
//         return ErrorPageGenerator::GenerateErrorResponse(HttpResponse::internalServerError("No se pudo crear el archivo en: " + fullFilePath));
//     }

//     out.write(request.getBody().c_str(), request.getBody().size());
//     out.close();

//     std::stringstream responseMsg;
//     responseMsg << "Archivo guardado correctamente como " << filename << "\n";

//     return HttpResponse::ok(responseMsg.str(), "text/plain");
// }

HttpResponse HttpPostRequestHandler::handleMethod(const HttpRequest& request, const Location* location, const Server* server) const
{
    if (location && !isMethodAllowed("POST", *location))
        return ErrorPageGenerator::GenerateErrorResponse(HttpResponse::methodNotAllowed());

    std::string requestPath = request.getUrl();
    if (requestPath.empty())
        requestPath = "/";

    if (CgiHandler::isCgiRequest(requestPath, location))
        return CgiHandler::execute(request, location, server);

    if (FileUploadHandler::isUploadRequest(request, location))
        return FileUploadHandler::handleUpload(request, location, server);

    return HttpResponse::ok("Data received successfully", "text/plain");
}
