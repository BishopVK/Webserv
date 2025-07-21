#pragma once

#include "HttpResponse.hpp"

class ErrorPageGenerator
{
  private:
    ErrorPageGenerator();
    ErrorPageGenerator(const ErrorPageGenerator& other);
    ~ErrorPageGenerator();
    ErrorPageGenerator& operator=(const ErrorPageGenerator& other);

  public:
    static HttpResponse GenerateErrorResponse(const HttpResponse& response);
};
