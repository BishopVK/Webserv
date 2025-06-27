#ifndef ACONTROLLER_HPP
#define ACONTROLLER_HPP

#include "IController.hpp"
#include <string>

class AController : public IController
{
  private:
    std::string _route;

  protected:
    AController();
    AController(const std::string& route);
    AController(const AController& other);
    AController& operator=(const AController& other);

  public:
    virtual ~AController();

    const std::string& getRoute() const;

    virtual HttpResponse handleGet(const HttpRequest& request);
    virtual HttpResponse handlePost(const HttpRequest& request);
    virtual HttpResponse handleDelete(const HttpRequest& request);

    virtual bool supportsMethod(const std::string& method) const;
};

#endif
