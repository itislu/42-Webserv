#include "fileUtils.hpp"

#include <string>
#include <sys/stat.h> // for stat(), struct stat, and S_ISREG

bool isFile(const std::string& filepath)
{
  const char* const path = filepath.c_str();
  struct stat path_stat = {};

  if (stat(path, &path_stat) != 0) {
    return false;
  }
  return (S_ISREG(path_stat.st_mode));
}

std::string getFileExtension(const std::string& filepath)
{
  const std::string::size_type pos = filepath.find_last_of('.');
  if (pos == std::string::npos) {
    return "";
  }
  return filepath.substr(pos);
}
