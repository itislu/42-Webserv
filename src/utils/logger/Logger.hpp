#pragma once
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <libftpp/memory.hpp>

#include <fstream>
#include <map>
#include <ostream>
#include <string>

namespace logFiles {
const char* const general = "./log/general.log";
const char* const http = "./log/http.log";
}

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

  static Logger& getInstance(const char* filename);

  std::ostream& info();
  std::ostream& warning();
  std::ostream& error();

  ~Logger();

private:
  typedef std::map<const char*, ft::shared_ptr<Logger> > InstanceMap;

  Logger();
  explicit Logger(const char* filename);
  Logger(const Logger& other);
  Logger& operator=(const Logger& other);

  std::ostream& _log(LogLevel level);
  static InstanceMap& _instances();
  static std::string _currentTime();

  static const int _widthLevelStr = 7;

  std::ofstream _file;
};

#endif
