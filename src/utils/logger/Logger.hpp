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
private:
  class FileStream;

public:
  static Logger& getInstance(const char* filename) throw();

  FileStream& info();
  FileStream& warning();
  FileStream& error();

  ~Logger() {}

private:
  typedef std::map<const char*, ft::shared_ptr<Logger> > InstanceMap;

  enum LogLevel
  {
    INFO,
    WARNING,
    ERROR
  };

  class FileStream
  {
  public:
    template<typename T>
    FileStream& operator<<(const T& msg);

  private:
    friend class Logger;
    std::ofstream _stream;
  };

  Logger() throw() {}
  explicit Logger(const char* filename);
  Logger(const Logger& other);
  Logger& operator=(const Logger& other);

  FileStream& _log(LogLevel level);
  static InstanceMap& _instances();
  static std::string _currentTime();

  static const int _widthLevelStr = 7;

  FileStream _file;
};

template<typename T>
Logger::FileStream& Logger::FileStream::operator<<(const T& msg)
{
  _stream << msg;
  return *this;
}

#endif
