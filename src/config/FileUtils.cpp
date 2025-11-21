#include "FileUtils.hpp"
#include <string>
#include <sys/stat.h> // for stat(), struct stat, and S_ISREG
#include <unistd.h>

namespace config {
namespace fileutils {

bool isFile(const std::string& filepath)
{
  const char* const path = filepath.c_str();
  struct stat path_stat = {};

  if (stat(path, &path_stat) != 0) {
    return false;
  }
  return (S_ISREG(path_stat.st_mode));
}

bool isDirectory(const std::string& filepath)
{
  const char* const path = filepath.c_str();
  struct stat path_stat = {};

  if (stat(path, &path_stat) != 0) {
    return false;
  }
  return (S_ISDIR(path_stat.st_mode));
}

/* ACCESS */
bool isExecuteable(const std::string& path)
{
  return (access(path.c_str(), X_OK) == 0);
}

bool isReadable(const std::string& path)
{
  return (access(path.c_str(), R_OK) == 0);
}

} // namespace fileutils
} // namespace config
