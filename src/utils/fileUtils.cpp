#include "fileUtils.hpp"

#include <libftpp/optional.hpp>
#include <libftpp/string.hpp>
#include <libftpp/utility.hpp>

#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sys/stat.h> // for stat(), struct stat, and S_ISREG
#include <unistd.h>

bool isFile(const std::string& filepath)
{
  const char* const path = filepath.c_str();
  struct stat path_stat = {};

  // stat returns 0 on success, -1 on error
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

bool isWriteable(const std::string& path)
{
  return (access(path.c_str(), W_OK) == 0);
}

ft::optional<std::size_t> getFileSize(const std::string& filepath)
{
  struct stat info = {};

  // stat returns 0 on success, -1 on error
  if (stat(filepath.c_str(), &info) != 0) {
    return ft::nullopt;
  }
  if (info.st_size < 0) {
    return ft::nullopt;
  }

  return static_cast<std::size_t>(info.st_size);
}

std::string getFileExtension(const std::string& filepath)
{
  const std::string::size_type pos = filepath.find_last_of('.');
  if (pos == std::string::npos) {
    return "";
  }
  return filepath.substr(pos);
}

// NOLINTBEGIN(bugprone-random-generator-seed, misc-predictable-rand)
std::string getRandomFileName(const std::string& prefix)
{
  static bool seeded = false;

  if (!seeded) {
    seeded = true;
    std::srand(std::time(FT_NULLPTR));
  }

  std::string filename;
  filename.append(prefix);
  filename.append(ft::to_string(std::rand()));
  filename.append(ft::to_string(std::rand()));
  return filename;
}
// NOLINTEND(bugprone-random-generator-seed, misc-predictable-rand)
