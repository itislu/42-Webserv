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
// PUBLIC

Logger& Logger::getInstance(const char* filename) throw()
{
  static Logger emptyLogger;

  try {
    // Existing Logger
    const InstanceMap::iterator iter = _instances().find(filename);
    if (iter != _instances().end()) {
      return *iter->second;
    }

    // New Logger
    const ft::shared_ptr<Logger> loggerPtr(new Logger(filename));
    _instances()[filename] = loggerPtr;
    return *loggerPtr;
  } catch (...) {
    return emptyLogger;
  }
}

Logger::FileStream& Logger::info()
{
  return _log(INFO);
}
Logger::FileStream& Logger::warning()
{
  return _log(WARNING);
}
Logger::FileStream& Logger::error()
{
  return _log(ERROR);
}

/* ***************************************************************************/
// PRIVATE
Logger::Logger(const char* filename)
{
  _file._stream.open(filename, std::ios::out | std::ios::trunc);
  _file << std::unitbuf; // enables automatic flush
  if (!_file._stream) {
    std::cerr << "Failed to open log file: " << filename << '\n';
  }
}

Logger::FileStream& Logger::_log(LogLevel level)
{
  if (!_file._stream.is_open()) {
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
