#pragma once

#include <string>

class File
{
  private:
    std::string _name;
    std::string _content;

  public:
    File();
    File(const std::string& name, const std::string& content);
    File(const File& other);
    File& operator=(const File& other);
    ~File();

    const std::string& getName() const;
    const std::string& getContent() const;
    void               setName(const std::string& name);
    void               setContent(const std::string& content);
};
