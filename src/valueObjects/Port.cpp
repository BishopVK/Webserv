#include "Port.hpp"

Port::Port() : IntValue()
{
}

Port::~Port()
{
}

Port::Port(int value) : IntValue(value)
{
    if (value < 0 || value > 65535)
    {
        throw InvalidPortException();
    }
}

Port::Port(const Port& other) : IntValue(other)
{
}

Port& Port::operator=(const Port& other)
{
    if (this != &other)
    {
        IntValue::operator=(other);
    }
    return *this;
}

const char* Port::InvalidPortException::what(std::string& message) const throw()
{
    return message.c_str();
}

const char* Port::InvalidPortException::what() const throw()
{
    return "Invalid port number. Port must be between 0 and 65535.";
}
