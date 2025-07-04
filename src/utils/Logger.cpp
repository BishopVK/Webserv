#include "../../include/utils/Logger.hpp"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

Logger& Logger::instance()
{
    static Logger _instance;
    return _instance;
}

Logger::Logger() : _toFile(false)
{
    _level_strings.insert(std::make_pair(INFO, "INFO"));
    _level_strings.insert(std::make_pair(WARNING, "WARNING"));
    _level_strings.insert(std::make_pair(ERROR, "ERROR"));
    _level_strings.insert(std::make_pair(DEBUG, "DEBUG"));
}

Logger::~Logger()
{
    if (_file.is_open())
        _file.close();
}

void Logger::setLogFile(const std::string& filename)
{
    if (_file.is_open())
        _file.close();
    _file.open(filename.c_str(), std::ios::app);
    _toFile = _file.is_open();
    if (!_toFile)
        std::cerr << "Logger: No se pudo abrir el archivo de log: " << filename << std::endl;
}

void Logger::log(Level level, const std::string& message)
{
    std::ostringstream oss;
    oss << "[" << getTimestamp() << "] "
        << "[" << levelToString(level) << "] " << message << std::endl;

    if (_toFile && _file.is_open())
    {
        _file << oss.str();
        _file.flush();
    }
    else
        std::cout << oss.str();
}

void Logger::info(const std::string& message)
{
    log(INFO, message);
}

void Logger::warning(const std::string& message)
{
    log(WARNING, message);
}

void Logger::error(const std::string& message)
{
    log(ERROR, message);
}

void Logger::debug(const std::string& message)
{
    log(DEBUG, message);
}

std::string Logger::levelToString(Level level) const
{
    std::map<Level, std::string>::const_iterator it = _level_strings.find(level);
    return it != _level_strings.end() ? it->second : "UNKNOWN";
}

std::string Logger::getTimestamp() const
{
    std::ostringstream oss;
    std::time_t        t = std::time(NULL);
    std::tm*           tm = std::localtime(&t);
    oss << std::setfill('0') << std::setw(2) << tm->tm_hour << ":" << std::setw(2) << tm->tm_min << ":" << std::setw(2) << tm->tm_sec;
    return oss.str();
}
