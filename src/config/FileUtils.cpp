#include "FileUtils.hpp"
#include <cstddef>
#include <string>
#include <sys/stat.h> // for stat(), struct stat, and S_ISREG

bool fileutils::checkFileExtension(const std::string& filepath,
                                   const std::string& ext)
{
  if (filepath.size() <= ext.size()) {
    return false;
  }
  const std::size_t dotPos = filepath.find_last_of('.');
  if (dotPos != std::string::npos) {
    const std::string fileExt =
      filepath.substr(dotPos, filepath.size() - dotPos);
    return fileExt == ext;
  }
  return false;
}

bool fileutils::isFile(const std::string& filepath)
{
  const char* const path = filepath.c_str();
  struct stat path_stat = {};

  if (stat(path, &path_stat) != 0) {
    return false;
  }
  return (S_ISREG(path_stat.st_mode));
}
