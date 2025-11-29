#pragma once
#ifndef UTILS_CONVERT_HPP
#define UTILS_CONVERT_HPP

#include <cstddef>
#include <string>

namespace utils {

/**
 * Returns `-1` if `chr` is not a valid hexadecimal character.
 */
int hexToInt(char chr);
int toInt(const std::string& str);
long toLong(const std::string& str);
std::size_t toSizeT(const std::string& str);

} // namespace utils

#endif
