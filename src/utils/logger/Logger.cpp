#include "Logger.hpp"

#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>

#include <ctime>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <map>
#include <string>

/* ************************************************************************** */
// INIT

bool Logger::_loggerEnabled = false;

/* ************************************************************************** */
// PUBLIC

Logger& Logger::getInstance(const char* filename) throw()
{
  static Logger emptyLogger;

  try {
    const std::string filenameStr(filename);

    // Existing Logger
    const InstanceMap::iterator iter = _instances().find(filenameStr);
    if (iter != _instances().end()) {
      return *iter->second;
    }

    // New Logger
    const ft::shared_ptr<Logger> loggerPtr(new Logger(filenameStr));
    _instances()[filenameStr] = loggerPtr;
    return *loggerPtr;
  } catch (...) {
    return emptyLogger;
  }
}

std::ostream& Logger::info()
{
  return _log(INFO);
}
std::ostream& Logger::warning()
{
  return _log(WARNING);
}
std::ostream& Logger::error()
{
  return _log(ERROR);
}

void Logger::enableLogging()
{
  _loggerEnabled = true;
}

void Logger::disableLogging()
{
  _loggerEnabled = false;
}

/* ***************************************************************************/
// PRIVATE
Logger::Logger(const std::string& filename)
{
  _file.open(filename.c_str(), std::ios::out | std::ios::trunc);
  _file << std::unitbuf; // enables automatic flush
  if (!_file) {
    std::cerr << "Failed to open log file: " << filename << '\n';
  }
}

std::ostream& Logger::_log(LogLevel level)
{
  if (!_file.is_open() || !_loggerEnabled) {
    return _file;
  }
  std::string levelStr;
  switch (level) {
    case INFO:
      levelStr = "INFO";
      break;
    case WARNING:
      levelStr = "WARNING";
      break;
    case ERROR:
      levelStr = "ERROR";
      break;
  }
  _file << "[" << _currentTime() << "] [" << std::setw(_widthLevelStr)
        << levelStr << "] ";
  return _file;
}

Logger::InstanceMap& Logger::_instances()
{
  static InstanceMap instances;
  return instances;
}

// NOLINTBEGIN(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
std::string Logger::_currentTime()
{
  // todo replace with lukas timestamp class
  std::time_t const now = std::time(FT_NULLPTR);
  const int bufSize = 32;
  char buff[bufSize];
  (void)std::strftime(
    buff, sizeof(buff), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
  return buff;
}
// NOLINTEND(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
