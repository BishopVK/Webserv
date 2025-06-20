#include "server/HttpServer.hpp"

int main()
{
    const std::string port = "8080";
    HttpServer        server(port);
    server.run();
    return 0;
}
