#ifndef FILE_UTILS_HPP
#define FILE_UTILS_HPP

#include <libftpp/optional.hpp>

#include <cstddef>
#include <string>

bool isFile(const std::string& filepath);
bool isDirectory(const std::string& filepath);
bool isExecuteable(const std::string& path);
bool isReadable(const std::string& path);
bool isWriteable(const std::string& path);

ft::optional<std::size_t> getFileSize(const std::string& filepath);
std::string getFileExtension(const std::string& filepath);
std::string getRandomFileName(const std::string& prefix);

#endif
