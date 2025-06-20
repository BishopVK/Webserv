#include "AController.hpp"
#include "../../http/HttpResponse.hpp"
#include <string>

AController::AController() : _route("/")
{
}

AController::AController(const std::string& route) : _route(route)
{
}

AController::AController(const AController& other) : _route(other._route)
{
    (void)other;
}

AController& AController::operator=(const AController& other)
{
    if (this != &other)
    {
        _route = other._route;
    }
    return *this;
}

AController::~AController()
{
}

const std::string& AController::getRoute() const
{
    return _route;
}

HttpResponse AController::handleGet(const HttpRequest& request)
{
    (void)request;
    return HttpResponse::methodNotAllowed();
}

HttpResponse AController::handlePost(const HttpRequest& request)
{
    (void)request;
    return HttpResponse::methodNotAllowed();
}

HttpResponse AController::handleDelete(const HttpRequest& request)
{
    (void)request;
    return HttpResponse::methodNotAllowed();
}

bool AController::supportsMethod(const std::string& method) const
{
    return (method == "GET" || method == "POST" || method == "DELETE");
}
