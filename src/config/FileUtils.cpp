#include "FileUtils.hpp"
#include <cstddef>
#include <string>
#include <sys/stat.h> // for stat(), struct stat, and S_ISREG

namespace config {

bool fileutils::isFile(const std::string& filepath)
{
  const char* const path = filepath.c_str();
  struct stat path_stat = {};

  if (stat(path, &path_stat) != 0) {
    return false;
  }
  return (S_ISREG(path_stat.st_mode));
}

} // namespace config
