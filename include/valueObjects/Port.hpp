#pragma once

#include <string>

class Port
{
  private:
    int  _value;
    bool _is_null;

  public:
    Port();
    ~Port();
    Port(int value);
    Port(const Port& other);
    Port& operator=(const Port& other);

    int  getValue() const;
    void setValue(int value);

    bool isNull() const;
    void setNull(bool is_null);

    bool hasValue() const;

    int         toInt() const;
    std::string toString() const;

    explicit operator bool() const;
};
