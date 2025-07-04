#pragma once
#include <fstream>
#include <map>
#include <string>

class Logger
{
  public:
    enum Level
    {
        INFO,
        WARNING,
        ERROR,
        DEBUG
    };

  private:
    std::ofstream _file;
    bool          _toFile;
    std::map<Level, std::string> _level_strings;

    Logger();
    Logger(const Logger&);
    Logger& operator=(const Logger&);
    ~Logger();

    std::string levelToString(Level level) const;
    std::string getTimestamp() const;

  public:
    static Logger& instance();

    void log(Level level, const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    void debug(const std::string& message);

    void setLogFile(const std::string& filename);
};
