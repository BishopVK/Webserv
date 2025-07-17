#include "LocationMatcher.hpp"
#include "../utils/PathHandler.hpp"

LocationMatcher::LocationMatcher()
{
}

LocationMatcher::LocationMatcher(const LocationMatcher& other)
{
    (void)other;
}

LocationMatcher& LocationMatcher::operator=(const LocationMatcher& other)
{
    (void)other;
    return *this;
}

LocationMatcher::~LocationMatcher()
{
}

const Location* LocationMatcher::findBestMatch(const std::string& requestPath, const std::vector<Location>& locations)
{
    if (locations.empty())
        return NULL;

    const Location* exactMatch = findExactMatch(requestPath, locations);
    if (exactMatch)
        return exactMatch;

    const Location* prefixMatch = findPrefixMatch(requestPath, locations);
    if (prefixMatch)
        return prefixMatch;

    return NULL;
}

const Location* LocationMatcher::findExactMatch(const std::string& requestPath, const std::vector<Location>& locations)
{
    std::string normalizedRequest = PathHandler::normalizeUrlPath(requestPath);

    for (std::vector<Location>::const_iterator it = locations.begin(); it != locations.end(); ++it)
    {
        std::string normalizedLocation = PathHandler::normalizeUrlPath(it->getRoute());
        if (normalizedRequest == normalizedLocation)
            return &(*it);
    }

    return NULL;
}

const Location* LocationMatcher::findPrefixMatch(const std::string& requestPath, const std::vector<Location>& locations)
{
    std::string     normalizedRequest = PathHandler::normalizeUrlPath(requestPath);
    const Location* bestMatch = NULL;
    int             bestScore = -1;

    for (std::vector<Location>::const_iterator it = locations.begin(); it != locations.end(); ++it)
    {
        std::string normalizedLocation = PathHandler::normalizeUrlPath(it->getRoute());
        if (PathHandler::isPrefixMatch(normalizedRequest, normalizedLocation))
        {
            int score = static_cast<int>(normalizedLocation.length());
            if (score > bestScore)
            {
                bestScore = score;
                bestMatch = &(*it);
            }
        }
    }

    return bestMatch;
}

bool LocationMatcher::isPathMatch(const std::string& requestPath, const std::string& locationPath)
{
    std::string normalizedRequest = PathHandler::normalizeUrlPath(requestPath);
    std::string normalizedLocation = PathHandler::normalizeUrlPath(locationPath);

    return ((normalizedRequest == normalizedLocation) || PathHandler::isPrefixMatch(normalizedRequest, normalizedLocation));
}

std::string LocationMatcher::getMatchedPath(const std::string& requestPath, const std::string& locationPath)
{
    std::string normalizedRequest = PathHandler::normalizeUrlPath(requestPath);
    std::string normalizedLocation = PathHandler::normalizeUrlPath(locationPath);

    if (normalizedRequest == normalizedLocation)
        return normalizedLocation;

    if (PathHandler::isPrefixMatch(normalizedRequest, normalizedLocation))
        return normalizedLocation;

    return "";
}
