#include "Logger.hpp"

#include <cstddef>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

/* ************************************************************************** */
// PUBLIC
Logger::Logger(const char* filename)
{
  _file.open(filename, std::ios::out | std::ios::trunc);
  if (!_file) {
    std::cerr << "Failed to open log file: " << filename << '\n';
  }
}

Logger::~Logger()
{
  if (_file.is_open()) {
    _file.close();
  }
}

// NOLINTBEGIN(performance-avoid-endl)
void Logger::log(LogLevel level, const std::string& message)
{
  if (!_file) {
    return;
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
  _file << "[" << currentTime() << "] [";
  _file << std::setw(_widthLevelStr) << levelStr << "] ";
  _file << message << std::endl;
}
// NOLINTEND(performance-avoid-endl)

void Logger::info(const std::string& msg)
{
  log(INFO, msg);
}
void Logger::warning(const std::string& msg)
{
  log(WARNING, msg);
}
void Logger::error(const std::string& msg)
{
  log(ERROR, msg);
}

/* ************************************************************************** */
// PRIVATE

// NOLINTBEGIN(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
std::string Logger::currentTime()
{
  time_t const now = time(0);
  const int bufSize = 32;
  char buf[bufSize];
  const std::size_t res =
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
  (void)res;
  return buf;
}
// NOLINTEND(cppcoreguidelines-pro-bounds-array-to-pointer-decay)
