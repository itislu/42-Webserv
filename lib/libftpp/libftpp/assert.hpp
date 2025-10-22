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

#	include "libftpp/type_traits.hpp"

namespace ft {

/**
 * @brief Provides a dependent `false` value
 *
 * Useful to cause a compilation error if a template gets instantiated.
 * By making a static assertion that always fails, a free-standing function can
 * essentially be disabled.
 * One template argument must be a template type itself, otherwise the compiler
 * detects the assertion failure in the first phase of two-phase name lookup.
 * More template arguments can be passed to prevent unused typedef warnings, for
 * example.
 */
template <typename,
          typename = void,
          typename = void,
          typename = void,
          typename = void,
          typename = void,
          typename = void,
          typename = void,
          typename = void,
          typename = void>
struct make_false : ft::false_type {};

/**
 * @brief Provides a dependent `true` value
 *
 * Useful to prevent unused typedef warnings by using the type in a static
 * assertion that always passes.
 */
template <typename,
          typename = void,
          typename = void,
          typename = void,
          typename = void,
          typename = void,
          typename = void,
          typename = void,
          typename = void,
          typename = void>
struct make_true : ft::true_type {};

} // namespace ft

#endif // LIBFTPP_ASSERT_HPP
