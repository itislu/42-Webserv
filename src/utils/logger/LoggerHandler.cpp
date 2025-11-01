
#include "LoggerHandler.hpp"

#include "utils/logger/Logger.hpp"

#include <exception>
#include <map>

/* ************************************************************************** */
// INIT

// std::map<const char*, Logger*> LoggerHandler::_instances;

/* ************************************************************************** */
// PUBLIC
Logger& LoggerHandler::getInstance(const char* filename)
{
  static LoggerHandler singelton;

  try {
    return *singelton._instances.at(filename);
  } catch (const std::exception& e) {
    Logger* const logger = new Logger(filename);
    singelton._instances[filename] = logger;
    return *logger;
  }
  return *singelton._instances[filename];
}

/* ************************************************************************** */
// PRIVATE

LoggerHandler::LoggerHandler() {}

LoggerHandler::~LoggerHandler()
{
  for (InstanceMap::const_iterator iter = _instances.begin();
       iter != _instances.end();
       ++iter) {
    delete iter->second;
  }
}
