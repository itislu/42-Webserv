#ifndef FILEUTILS_HPP
#define FILEUTILS_HPP

#include <string>

bool checkFileExtension(const std::string& filepath, const std::string& ext);
bool isFile(const std::string& filepath);

#endif
