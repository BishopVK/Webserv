#include "ErrorPageGenerator.hpp"
#include "HttpResponse.hpp"
#include <sstream>
#include <string>

HttpResponse ErrorPageGenerator::GenerateErrorResponse(const HttpResponse& response)
{
    std::stringstream html;

    html << "<!DOCTYPE html>\n<html>\n<head>\n"
         << "<title>Error " << response.getStatusCode() << "</title>\n"
         << "</head>\n<body>\n"
         << "<h1>Error " << response.getStatusCode() << "</h1>\n"
         << "<p>" << response.getReasonPhrase() << "</p>\n"
         << "</body>\n</html>\n";

    HttpResponse errorResponse = response;
    errorResponse.setHeader("Content-Type", "text/html");
    errorResponse.setBody(html.str());
    
    return errorResponse;
}
