#pragma once
#ifndef LIBFTPP_UTILITY_HPP
#	define LIBFTPP_UTILITY_HPP

#	include "libftpp/utility/countof.ipp"
#	include "libftpp/movable.hpp"
#	include "libftpp/type_traits.hpp"
#	include <string>

/**
 * https://en.cppreference.com/w/cpp/utility/unreachable
 */
#	if defined(__GNUC__) || defined(__clang__)
#		define FT_UNREACHABLE() __builtin_unreachable()
#	elif defined(_MSC_VER)
#		define FT_UNREACHABLE() __assume(false)
#	else // defined(__GNUC__) || defined(__clang__)
#		include <exception>
#		define FT_UNREACHABLE() std::terminate()
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

/* demangle */

std::string demangle(const char* mangled_name);

/**
 * @brief Casts an lvalue to an rvalue reference emulation to indicate an object
 * may be moved from
 *
 * Needed for functions taking `ft::rvalue<T>&` parameters to be chosen by
 * overload resolution.
 *
 * `ft::remove_reference<T&>::type` is used to prevent nesting of `ft::rvalue`s.
 *
 * https://en.cppreference.com/w/cpp/utility/move
 */
template <typename T>
ft::rvalue<typename ft::remove_reference<T&>::type>& move(T& t) throw();

} // namespace ft

#	include "libftpp/utility/move.tpp" // IWYU pragma: export

#endif // LIBFTPP_UTILITY_HPP
