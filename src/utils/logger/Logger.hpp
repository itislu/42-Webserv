#pragma once
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <libftpp/memory.hpp>

#include <fstream>
#include <map>
#include <string>

#define LOG_GENERAL "./log/general.log"
#define LOG_HTTP "./log/http.log"

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

  static Logger& getInstance(const char* filename) throw();

  Logger& info();
  Logger& warning();
  Logger& error();

  ~Logger() {}

private:
  typedef std::map<const char*, ft::shared_ptr<Logger> > InstanceMap;

  Logger() throw() {}
  explicit Logger(const char* filename);
  Logger(const Logger& other);
  Logger& operator=(const Logger& other);

  Logger& _log(LogLevel level);
  static InstanceMap& _instances();
  static std::string _currentTime();

  template<typename T>
  friend Logger& operator<<(Logger& logger, const T& msg);

  static const int _widthLevelStr = 7;

  std::ofstream _file;
};

template<typename T>
Logger& operator<<(Logger& logger, const T& msg)
{
  logger._file << msg;
  return logger;
}

#endif
