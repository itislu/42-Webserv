#pragma once
#ifndef LIBFTPP_UTILITY_HPP
#	define LIBFTPP_UTILITY_HPP

#	include "libftpp/utility/countof.ipp"
#	include <string>

/**
 * https://en.cppreference.com/w/cpp/utility/unreachable
 */
#	if defined(__GNUC__) || defined(__clang__)
#		define FT_UNREACHABLE() (__builtin_unreachable())
#	elif defined(_MSC_VER)
#		define FT_UNREACHABLE() (__assume(false))
#	else // defined(__GNUC__) || defined(__clang__)
#		include <exception>
#		define FT_UNREACHABLE() (std::terminate())
#	endif // defined(__GNUC__) || defined(__clang__)

/**
 * @brief Macro to get the length of a C-array at compile time in a type-safe
 * manner
 *
 * Inspiration:
 * https://web.archive.org/web/20141223215212/http://www.drdobbs.com/cpp/counting-array-elements-at-compile-time/197800525?pgno=6
 */
#	define FT_COUNTOF(ARRAY) LIBFTPP_COUNTOF_IMPL(ARRAY)

namespace ft {

std::string demangle(const char* mangled_name);

} // namespace ft

#endif // LIBFTPP_UTILITY_HPP
