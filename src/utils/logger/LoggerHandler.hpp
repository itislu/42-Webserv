#pragma once
#ifndef LOGGER_INSTANCE_HPP
#define LOGGER_INSTANCE_HPP

#include <utils/logger/Logger.hpp>

#include <map>

namespace log {
const char* const general = "./log/general.log";
const char* const http = "./log/http.log";
}

/* ************************************************************************** */
class LoggerHandler
{
public:
  static Logger& getInstance(const char* filename);

private:
  LoggerHandler();
  ~LoggerHandler();
  LoggerHandler(const LoggerHandler& other);
  LoggerHandler& operator=(const LoggerHandler& other);

  typedef std::map<const char*, Logger*> InstanceMap;
  InstanceMap _instances;
};

#endif
