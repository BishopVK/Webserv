#include "../../include/valueObjects/IntValue.hpp"
#include <sstream>

IntValue::IntValue() : _value(0), _is_null(true)
{
}
IntValue::IntValue(int value) : _value(value), _is_null(false)
{
}
IntValue::IntValue(const IntValue& other) : _value(other._value), _is_null(other._is_null)
{
}

IntValue& IntValue::operator=(const IntValue& other)
{
    if (this != &other)
    {
        _value = other._value;
        _is_null = other._is_null;
    }
    return *this;
}

IntValue::~IntValue()
{
}

int IntValue::getValue() const
{
    return _value;
}

void IntValue::setValue(int value)
{
    _value = value;
    _is_null = false;
}

bool IntValue::isNull() const
{
    return _is_null;
}

void IntValue::setNull(bool is_null)
{
    _is_null = is_null;
}

bool IntValue::hasValue() const
{
    return !_is_null;
}

int IntValue::toInt() const
{
    return _value;
}

std::string IntValue::toString() const
{
    std::ostringstream oss;
    if (_is_null)
        return "null";
    oss << _value;
    return oss.str();
}
