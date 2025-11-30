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
#define LOG_SERVER "./log/server.log"

/* ************************************************************************** */
class Logger
{
public:
  ~Logger() {}

  static Logger& getInstance(const char* filename) throw();

  std::ostream& info();
  std::ostream& warning();
  std::ostream& error();

  static void enableLogging();
  static void disableLogging();

private:
  static bool _loggerEnabled;
  typedef std::map<std::string, ft::shared_ptr<Logger> > InstanceMap;

  enum LogLevel
  {
    INFO,
    WARNING,
    ERROR
  };

  Logger() throw();
  explicit Logger(const std::string& filename);
  Logger(const Logger& other);
  Logger& operator=(const Logger& other);

  bool _openFile();
  std::ostream& _log(LogLevel level);
  static InstanceMap& _instances();
  static std::string _currentTime();

  static const int _widthLevelStr = 7;

  std::ofstream _file;
  std::string _filename;
  bool _isFileCreated;
};

#endif
