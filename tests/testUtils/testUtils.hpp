#pragma once
#ifndef TEST_UTILS
#define TEST_UTILS

#include <cstddef>
#include <string>

namespace testUtils {

/**
 * @brief Creates a `std::string` from a character array, excluding the final
 * NUL terminator.
 *
 * Useful for making strings with embedded NUL characters.
 *
 * Example:
 * @code
 * testUtils::makeString("foo\x00bar"); // Returns std::string of length 7.
 * @endcode
 */
// NOLINTBEGIN(cppcoreguidelines-avoid-c-arrays)
template<std::size_t N>
std::string makeString(const char (&arr)[N]);
// NOLINTEND(cppcoreguidelines-avoid-c-arrays)

} // namespace testUtils

#include "testUtils.tpp" // IWYU pragma: export

#endif
