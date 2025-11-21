#ifndef FILEUTILS_HPP
#define FILEUTILS_HPP

#include <string>

namespace config {
namespace fileutils {

bool isFile(const std::string& filepath);
bool isDirectory(const std::string& filepath);

bool isExecuteable(const std::string& path);
bool isReadable(const std::string& path);

} // namespace fileutils
} // namespace config

#endif
