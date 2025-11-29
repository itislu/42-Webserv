#pragma once
#ifndef UTILS_CONVERT_HPP
#define UTILS_CONVERT_HPP

#include <string>

namespace utils {

/**
 * Returns `-1` if `chr` is not a valid hexadecimal character.
 */
int hexToInt(char chr);
template<typename To>
To toNumber(const std::string& str);

} // namespace utils

#include "convert.tpp" // IWYU pragma: export

#endif
