#include "server/HttpServer.hpp"
#include <vector>

int main()
{
    std::vector<std::string> ports;
    ports.push_back("8080");
    ports.push_back("8081");
    ports.push_back("8082");

    HttpServer server(ports);
    server.run();
    return 0;
}
