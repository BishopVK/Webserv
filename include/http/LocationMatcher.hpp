#ifndef LOCATION_MATCHER_HPP
#define LOCATION_MATCHER_HPP

#include "Location.hpp"
#include <string>
#include <vector>

class LocationMatcher
{
  private:
    LocationMatcher();
    LocationMatcher(const LocationMatcher& other);
    LocationMatcher& operator=(const LocationMatcher& other);
    ~LocationMatcher();

  public:
    static const Location* findBestMatch(const std::string& requestPath, const std::vector<Location>& locations);
    static const Location* findExactMatch(const std::string& requestPath, const std::vector<Location>& locations);
    static const Location* findPrefixMatch(const std::string& requestPath, const std::vector<Location>& locations);
    static bool            isPathMatch(const std::string& requestPath, const std::string& locationPath);
    static std::string     getMatchedPath(const std::string& requestPath, const std::string& locationPath);
};

#endif
