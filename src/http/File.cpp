#include "File.hpp"

File::File() : _name(""), _content("")
{
}

File::File(const std::string& name, const std::string& content) : _name(name), _content(content)
{
}

File::File(const File& other) : _name(other._name), _content(other._content)
{
}

File& File::operator=(const File& other)
{
    if (this != &other)
    {
        _name = other._name;
        _content = other._content;
    }
    return *this;
}

File::~File()
{
}

const std::string& File::getName() const
{
    return _name;
}

const std::string& File::getContent() const
{
    return _content;
}

void File::setName(const std::string& name)
{
    _name = name;
}

void File::setContent(const std::string& content)
{
    _content = content;
}
