#pragma once
#ifndef UTILS_CONVERT_HPP
#define UTILS_CONVERT_HPP

#include <string>

namespace utils {

/**
 * @returns `-1` If `chr` is not a valid hexadecimal character.
 */
int hexToInt(char chr);

/**
 * @throws std::invalid_argument If `str` cannot be fully converted to type
 * `To`.
 */
template<typename To>
To toNumber(const std::string& str);

} // namespace utils

#include "convert.tpp" // IWYU pragma: export

#endif
