#ifndef AUTO_INDEX_GENERATOR_HPP
#define AUTO_INDEX_GENERATOR_HPP

#include <string>

class AutoIndexGenerator
{
  // private:
  // public:
  //   static std::string generateHtml(const std::string& requestPath, const std::string& physicalPath);
  
  private:
    std::string _requestPath;
    std::string _physicalPath;
  
  public:
    AutoIndexGenerator(const std::string& requestPath, const std::string& physicalPath);
    AutoIndexGenerator(const AutoIndexGenerator& other);
    ~AutoIndexGenerator();
    AutoIndexGenerator& operator=(const AutoIndexGenerator& other);
    
    std::string generateHtml() const;
};

#endif
