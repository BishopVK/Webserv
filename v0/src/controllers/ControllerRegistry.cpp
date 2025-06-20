#include "ControllerRegistry.hpp"
#include "base/AController.hpp"
#include "base/IController.hpp"
#include "TaskController.hpp"
#include "UserController.hpp"

ControllerRegistry::ControllerRegistry()
{
    _controllers.push_back(new TaskController);
    _controllers.push_back(new UserController);
}

ControllerRegistry::ControllerRegistry(const ControllerRegistry& other)
{
    (void)other;
}

ControllerRegistry& ControllerRegistry::operator=(const ControllerRegistry& other)
{
    (void)other;
    return *this;
}

ControllerRegistry::~ControllerRegistry()
{
    for (std::vector<AController*>::iterator it = _controllers.begin(); it != _controllers.end(); ++it)
        delete *it;
}

HttpResponse ControllerRegistry::processRequest(const HttpRequest& request)
{
    const std::string& url = request.getUrl();

    for (std::vector<AController*>::iterator it = _controllers.begin(); it != _controllers.end(); ++it)
    {
        AController* controller = *it;
        if (matchesRoute(url, controller->getRoute()))
        {
            return handleControllerRequest(*controller, request);
        }
    }

    return HttpResponse::notFound();
}

HttpResponse ControllerRegistry::handleControllerRequest(IController& controller, const HttpRequest& request)
{
    const std::string& method = request.getMethod();

    if (!controller.supportsMethod(method))
        return HttpResponse::methodNotAllowed();

    if (method == "GET")
        return controller.handleGet(request);
    else if (method == "POST")
        return controller.handlePost(request);
    else if (method == "DELETE")
        return controller.handleDelete(request);
    else
        return HttpResponse::methodNotAllowed();
}

bool ControllerRegistry::matchesRoute(const std::string& url, const std::string& routePrefix) const
{
    return url.find(routePrefix) == 0;
}
