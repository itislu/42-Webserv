#pragma once
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <libftpp/memory.hpp>

#include <fstream>
#include <map>
#include <ostream>
#include <string>

#define LOG_GENERAL "./log/general.log"
#define LOG_HTTP "./log/http.log"

/* ************************************************************************** */
class Logger
{
public:
  static Logger& getInstance(const char* filename) throw();

  std::ostream& info();
  std::ostream& warning();
  std::ostream& error();

  ~Logger() {}

private:
  typedef std::map<std::string, ft::shared_ptr<Logger> > InstanceMap;

  enum LogLevel
  {
    INFO,
    WARNING,
    ERROR
  };

  Logger() throw() {}
  explicit Logger(const std::string& filename);
  Logger(const Logger& other);
  Logger& operator=(const Logger& other);

  std::ostream& _log(LogLevel level);
  static InstanceMap& _instances();
  static std::string _currentTime();

  static const int _widthLevelStr = 7;

  std::ofstream _file;
};

#endif
