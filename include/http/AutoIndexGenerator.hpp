#ifndef AUTO_INDEX_GENERATOR_HPP
#define AUTO_INDEX_GENERATOR_HPP

#include <string>

class AutoIndexGenerator
{
  public:
    static std::string generateHtml(const std::string& requestPath, const std::string& physicalPath);
};

#endif
