#include "ConfigParser.hpp"
#include <cstddef>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/stat.h> // for stat(), struct stat, and S_ISREG

ConfigParser::ConfigParser(const char* path)
{
  _filepath = static_cast<std::string>(path);
}

bool ConfigParser::checkFileExtension() const
{
  const long extLen = 5;
  if (_filepath.size() <= extLen) {
    return false;
  }
  const std::size_t dotPos = _filepath.find_last_of('.');
  if (dotPos != std::string::npos) {
    const std::string ext = _filepath.substr(dotPos, _filepath.size() - dotPos);
    return ext == ".conf";
  }
  return false;
}

bool ConfigParser::isFile() const
{
  const char* const path = _filepath.c_str();
  struct stat path_stat = {};

  if (stat(path, &path_stat) != 0) {
    return false;
  }
  return (S_ISREG(path_stat.st_mode));
}

bool ConfigParser::validateInputFile() const
{
  if (!checkFileExtension()) {
    throw std::invalid_argument("invalid file: " + _filepath);
  }

  if (!isFile()) {
    throw std::invalid_argument("invalid file: " + _filepath);
  }

  std::ifstream file(_filepath.c_str());
  if (!file.is_open()) {
    throw std::invalid_argument("couldn't open configfile: " + _filepath);
  }
  return true;
}

void ConfigParser::parse()
{
  validateInputFile();
}