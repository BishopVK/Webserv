#pragma once

#include <string>

class IntValue
{
  private:
    int  _value;
    bool _is_null;

  public:
    IntValue();
    IntValue(int value);
    IntValue(const IntValue& other);
    IntValue& operator=(const IntValue& other);
    ~IntValue();

    int  getValue() const;
    void setValue(int value);

    bool isNull() const;
    void setNull(bool is_null);
    bool hasValue() const;

    int         toInt() const;
    std::string toString() const;
};
