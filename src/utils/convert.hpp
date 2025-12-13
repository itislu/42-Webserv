#pragma once
#ifndef UTILS_CONVERT_HPP
#define UTILS_CONVERT_HPP

#include <netinet/in.h>
#include <string>

namespace utils {

/**
 * @returns `-1` If `chr` is not a valid hexadecimal character.
 */
int hexToInt(char chr);

/**
 * The difference to `ft::from_string` is that this function throws an exception
 * if there are excess characters after the parsed number.
 *
 * @throws std::invalid_argument If `str` contains invalid or excess characters,
 * or the parsed number would be out of range for type `To`.
 */
template<typename To>
To toNumber(const std::string& str);

/**
 * @throws std::invalid_argument If `str` does not represent a valid port
 * number (1-65535).
 */
in_port_t toPort(const std::string& str);

} // namespace utils

#include "convert.tpp" // IWYU pragma: export

#endif
