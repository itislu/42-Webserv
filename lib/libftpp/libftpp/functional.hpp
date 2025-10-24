#pragma once
#ifndef LIBFTPP_FUNCTIONAL_HPP
#	define LIBFTPP_FUNCTIONAL_HPP

namespace ft {

/**
 * @brief Provides a uniform way to query properties of function types
 *
 * Supports functions with up to 10 parameters, including variadic functions
 * and "abominable" function types.
 * No members are defined for non-function types (SFINAE-friendly).
 * Pointers and references to functions are not function types.
 *
 * Available members for function types:
 * - `arity`: number of parameters (excluding variadic part)
 * - `result_type`: return type
 * - `argument_type<N>::type`: type of the Nth parameter (0-indexed)
 *   - Only available when `N` >= `arity` - except for functions with no
 *     parameters, then `argument_type<0>::type` is `void`.
 *
 * Conceptually similar to `std::numeric_limits` and `std::iterator_traits`.
 * Member names are chosen to fit in with `<functional>`.
 * Non-standard.
 */
template <typename F>
struct function_traits;

/**
 * @brief Function object implementing `x == y`
 *
 * https://en.cppreference.com/w/cpp/utility/functional/equal_to
 */
template <typename T = void>
struct equal_to {
	bool operator()(const T& lhs, const T& rhs) const;
};

/**
 * @brief Function object implementing `x == y` deducing parameter types
 *
 * Adds C++14's `void` specialization which deduces the parameter types from the
 * arguments.
 *
 * @note Unlike C++14's version, the return type is fixed and not deduced.
 *
 * https://en.cppreference.com/w/cpp/utility/functional/equal_to_void
 */
template <>
struct equal_to<void> {
	typedef void is_transparent;

	template <typename T, typename U>
	bool operator()(const T& lhs, const U& rhs) const;
	template <typename T, typename U>
	bool operator()(T* a, U* b) const;
};

/**
 * @brief Function object implementing `x != y`
 *
 * https://en.cppreference.com/w/cpp/utility/functional/not_equal_to
 */
template <typename T = void>
struct not_equal_to {
	bool operator()(const T& lhs, const T& rhs) const;
};

/**
 * @brief Function object implementing `x != y` deducing parameter types
 *
 * Adds C++14's `void` specialization which deduces the parameter types from the
 * arguments.
 *
 * @note Unlike C++14's version, the return type is fixed and not deduced.
 *
 * https://en.cppreference.com/w/cpp/utility/functional/not_equal_to_void
 */
template <>
struct not_equal_to<void> {
	typedef void is_transparent;

	template <typename T, typename U>
	bool operator()(const T& lhs, const U& rhs) const;
	template <typename T, typename U>
	bool operator()(T* a, U* b) const;
};

/**
 * @brief Function object implementing `x > y`
 *
 * https://en.cppreference.com/w/cpp/utility/functional/greater
 */
template <typename T = void>
struct greater {
	bool operator()(const T& lhs, const T& rhs) const;
};

/**
 * @brief Function object implementing `x > y` deducing parameter types
 *
 * Adds C++14's `void` specialization which deduces the parameter types from the
 * arguments.
 *
 * @note Unlike C++14's version, the return type is fixed and not deduced.
 *
 * https://en.cppreference.com/w/cpp/utility/functional/greater_void
 */
template <>
struct greater<void> {
	typedef void is_transparent;

	template <typename T, typename U>
	bool operator()(const T& lhs, const U& rhs) const;
	template <typename T, typename U>
	bool operator()(T* a, U* b) const;
};

/**
 * @brief Function object implementing `x < y`
 *
 * https://en.cppreference.com/w/cpp/utility/functional/less
 */
template <typename T = void>
struct less {
	bool operator()(const T& lhs, const T& rhs) const;
};

/**
 * @brief Function object implementing `x < y` deducing parameter types
 *
 * Adds C++14's `void` specialization which deduces the parameter types from the
 * arguments.
 *
 * @note Unlike C++14's version, the return type is fixed and not deduced.
 *
 * https://en.cppreference.com/w/cpp/utility/functional/less_void
 */
template <>
struct less<void> {
	typedef void is_transparent;

	template <typename T, typename U>
	bool operator()(const T& lhs, const U& rhs) const;
	template <typename T, typename U>
	bool operator()(T* a, U* b) const;
};

/**
 * @brief Function object implementing `x >= y`
 *
 * https://en.cppreference.com/w/cpp/utility/functional/greater_equal
 */
template <typename T = void>
struct greater_equal {
	bool operator()(const T& lhs, const T& rhs) const;
};

/**
 * @brief Function object implementing `x >= y` deducing parameter types
 *
 * Adds C++14's `void` specialization which deduces the parameter types from the
 * arguments.
 *
 * @note Unlike C++14's version, the return type is fixed and not deduced.
 *
 * https://en.cppreference.com/w/cpp/utility/functional/greater_equal_void
 */
template <>
struct greater_equal<void> {
	typedef void is_transparent;

	template <typename T, typename U>
	bool operator()(const T& lhs, const U& rhs) const;
	template <typename T, typename U>
	bool operator()(T* a, U* b) const;
};

/**
 * @brief Function object implementing `x <= y`
 *
 * https://en.cppreference.com/w/cpp/utility/functional/less_equal
 */
template <typename T = void>
struct less_equal {
	bool operator()(const T& lhs, const T& rhs) const;
};

/**
 * @brief Function object implementing `x <= y` deducing parameter types
 *
 * Adds C++14's `void` specialization which deduces the parameter types from the
 * arguments.
 *
 * @note Unlike C++14's version, the return type is fixed and not deduced.
 *
 * https://en.cppreference.com/w/cpp/utility/functional/less_equal_void
 */
template <>
struct less_equal<void> {
	typedef void is_transparent;

	template <typename T, typename U>
	bool operator()(const T& lhs, const U& rhs) const;
	template <typename T, typename U>
	bool operator()(T* a, U* b) const;
};

} // namespace ft

#	include "libftpp/functional/comparisons.tpp"     // IWYU pragma: export
#	include "libftpp/functional/function_traits.tpp" // IWYU pragma: export

#endif // LIBFTPP_FUNCTIONAL_HPP
