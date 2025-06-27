#pragma once

#include "IntValue.hpp"
#include <exception>

class Port : public IntValue
{
  public:
    Port();
    ~Port();
    Port(int value);
    Port(const Port& other);
    Port& operator=(const Port& other);

    class InvalidPortException : public std::exception
    {
      public:
        virtual const char* what(std::string& message) const throw();
        virtual const char* what() const throw();
    };
};
