#ifndef FILE_UTILS_HPP
#define FILE_UTILS_HPP

#include <libftpp/optional.hpp>

#include <cstddef>
#include <string>

ft::optional<std::size_t> getFileSize(const std::string& filepath);
bool isFile(const std::string& filepath);
std::string getFileExtension(const std::string& filepath);
std::string getRandomFileName(const std::string& prefix);

#endif
