#pragma once
#ifndef LIBFTPP_ASSERT_HPP
#	define LIBFTPP_ASSERT_HPP

#	include "libftpp/assert/static_assert.ipp"

/**
 * @brief Performs a compile-time assertion that works in C++98
 *
 * Usage: `FT_STATIC_ASSERT(21 + 21 == 42);`
 *
 * A comment in the same line as FT_STATIC_ASSERT can be used as an error
 * message:
 * ```
 * FT_STATIC_ASSERT(ft::is_object<T>::value); // T must be an object type.
 * FT_STATIC_ASSERT( // T must not be a cv-qualified type.
 *     !ft::is_const<T>::value && !ft::is_volatile<T>::value);
 * ```
 *
 * Inspiration:
 * https://web.archive.org/web/20130323104707/https://www.drdobbs.com/compile-time-assertions/184401873
 */
#	define FT_STATIC_ASSERT(EXPR) LIBFTPP_STATIC_ASSERT_IMPL(EXPR)

#endif // LIBFTPP_ASSERT_HPP
