#include "Logger.hpp"

#include "libftpp/array.hpp"
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <map>
#include <string>

/* ************************************************************************** */
// INIT

const char* const Logger::_envVar = "WEBSERV_LOGGING";

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
  _loggerEnabled() = true;
}

void Logger::disableLogging()
{
  _loggerEnabled() = false;
}

/* ***************************************************************************/
// PRIVATE

Logger::Logger() throw()
  : _isFileCreated(false)
{
  _file << std::unitbuf;
}

Logger::Logger(const std::string& filename)
  : _filename(filename)
  , _isFileCreated(false)
{
  _file << std::unitbuf;
  _openFile();
}

bool Logger::_openFile()
{
  if (!_loggerEnabled() || _filename.empty()) {
    if (_file.is_open()) {
      _file.close();
    }
    return false;
  }
  if (_file.is_open()) {
    return true;
  }

  _file.open(_filename.c_str(),
             _isFileCreated ? std::ios::app : std::ios::trunc);
  if (!_file.is_open()) {
    std::cerr << "Failed to open log file: " << _filename << '\n';
    return false;
  }

  _isFileCreated = true;
  return true;
}

std::ostream& Logger::_log(LogLevel level)
{
  if (!_openFile()) {
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

bool& Logger::_loggerEnabled()
{
  static bool _loggerEnabled = _initLoggingFromEnv();
  return _loggerEnabled;
}

bool Logger::_initLoggingFromEnv() throw()
{
  const char* const envValue = std::getenv(_envVar);
  if (envValue == FT_NULLPTR) {
    return false;
  }

  const ft::array<const char*, 7> enablers = { "1",  "true", "True", "TRUE",
                                               "on", "On",   "ON" };
  for (unsigned i = 0; i < enablers.size(); ++i) {
    if (std::strcmp(envValue, enablers[i]) == 0) {
      return true;
    }
  }
  return false;
}
