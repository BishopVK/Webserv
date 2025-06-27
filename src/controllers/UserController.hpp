#ifndef USER_CONTROLLER_HPP
#define USER_CONTROLLER_HPP

#include "base/AController.hpp"

class UserController : public AController
{
  public:
    UserController();
    UserController(const UserController& other);
    UserController& operator=(const UserController& other);
    virtual ~UserController();

    virtual HttpResponse handleGet(const HttpRequest& request);
    virtual HttpResponse handlePost(const HttpRequest& request);

    virtual bool supportsMethod(const std::string& method) const;
};

#endif
