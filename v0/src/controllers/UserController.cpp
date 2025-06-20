#include "UserController.hpp"
#include <sstream>

UserController::UserController() : AController("/user")
{
}

UserController::UserController(const UserController& other) : AController(other)
{
    (void)other;
}

UserController& UserController::operator=(const UserController& other)
{
    (void)other;
    return *this;
}

UserController::~UserController()
{
}

HttpResponse UserController::handleGet(const HttpRequest& request)
{
    std::stringstream body;
    body << "<html><body>"
         << "<h1>User Management</h1>"
         << "<p>List of users would be displayed here.</p>"
         << "<p>Request URL: " << request.getUrl() << "</p>"
         << "<p>Supported methods: GET, POST</p>"
         << "</body></html>";

    return HttpResponse::ok(body.str(), "text/html");
}

HttpResponse UserController::handlePost(const HttpRequest& request)
{
    (void)request;

    std::stringstream body;
    body << "<html><body>"
         << "<h1>User Created</h1>"
         << "<p>New user would be created here.</p>"
         << "<p>This is a demonstration of POST handling.</p>"
         << "</body></html>";

    return HttpResponse::ok(body.str(), "text/html");
}

bool UserController::supportsMethod(const std::string& method) const
{
    return (method == "GET" || method == "POST");
}
