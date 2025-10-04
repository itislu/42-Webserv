#pragma once
#ifndef LIBFTPP_OPERATORS_HPP
#	define LIBFTPP_OPERATORS_HPP

namespace ft {
namespace operators {

/**
 * @brief Struct to inherit from to generate all remaining comparison operators
 * for the inheriting class
 *
 * The `operator<` has to be defined for the inheriting class.
 * To generate operators which compare with a different type, the `operater<`
 * has to be defined outside of the class, with that type once on the rhs and
 * once on the lhs.
 *
 * Usage:
 * 1. Implement `operator<` for your class.
 * 2. Inherit from this struct.
 *
 * This works because `comparison` acts like a tag, and by inheriting from it it
 * gives the inheriting class access to the ft::operators namespace, which
 * contains the function templates for the operators.
 */
struct comparison {};

template <typename T, typename U>
bool operator==(const T& lhs, const U& rhs);

template <typename T, typename U>
bool operator!=(const T& lhs, const U& rhs);

template <typename T, typename U>
bool operator>(const T& lhs, const U& rhs);

template <typename T, typename U>
bool operator<=(const T& lhs, const U& rhs);

template <typename T, typename U>
bool operator>=(const T& lhs, const U& rhs);

} // namespace operators
} // namespace ft

#	include "libftpp/operators/operators.tpp" // IWYU pragma: export

#endif // LIBFTPP_OPERATORS_HPP
