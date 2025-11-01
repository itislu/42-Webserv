#pragma once
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <string>

/* ************************************************************************** */
class Logger
{
public:
  enum LogLevel
  {
    INFO,
    WARNING,
    ERROR
  };

  explicit Logger(const char* filename);
  ~Logger();

  void log(LogLevel level, const std::string& message);
  void info(const std::string& msg);
  void warning(const std::string& msg);
  void error(const std::string& msg);

private:
  Logger(const Logger& other);
  Logger& operator=(const Logger& other);

  static std::string currentTime();
  static const int _widthLevelStr = 7;

  std::ofstream _file;
};

#endif
