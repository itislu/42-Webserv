#pragma once
#ifndef LIBFTPP_SAFE_BOOL_HPP
#	define LIBFTPP_SAFE_BOOL_HPP

#	include "libftpp/safe_bool/safe_bool_base.hpp"

namespace ft {

/**
 * @brief Mixin providing safe boolean test that does not participate in
 * unwanted expressions (safe bool problem)
 *
 * Needed since C++98 does not support the `explicit` keyword with conversion
 * operators yet (introduced in C++11).
 *
 * Implements the Safe Bool idiom using CRTP (Curiously Recurring Template
 * Pattern).
 * https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Safe_bool
 *
 * Usage:
 * 1. Publicly inherit from this class using CRTP idiom:
 *    `class YourClass : public safe_bool<YourClass>`.
 * 2. Implement `bool operator_bool() const` instead of `operator bool() const`.
 */
template <typename Derived = void>
class safe_bool : private _safe_bool::safe_bool_base {
public:
	// Implicit conversion wanted; pointer-to-member not convertible to any
	// other type except for `bool`.
	operator safe_bool_t() const; // NOLINT(google-explicit-constructor)

protected:
	safe_bool();
	~safe_bool();

private:
	safe_bool(const safe_bool& /*unused*/);
	safe_bool& operator=(const safe_bool& /*unused*/);
};

/**
 * @brief Mixin providing safe boolean test that does not participate in
 * unwanted expressions (safe bool problem)
 *
 * Needed since C++98 does not support the `explicit` keyword with conversion
 * operators yet (introduced in C++11).
 *
 * Implements the Safe Bool idiom using dynamic dispatch.
 * https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Safe_bool
 *
 * Usage:
 * 1. Publicly inherit from this class with no template parameter:
 *    `class YourClass : public safe_bool<>`.
 * 2. Implement `bool operator_bool() const` instead of `operator bool() const`.
 *
 * @note If your class is not virtual yet, prefer to pass your class as template
 * parameter (CRTP idiom) to avoid it becoming virtual.
 */
template <>
class safe_bool<void> : private _safe_bool::safe_bool_base {
public:
	// Implicit conversion wanted; pointer-to-member not convertible to any
	// other type except for `bool`.
	operator safe_bool_t() const; // NOLINT(google-explicit-constructor)

protected:
	safe_bool();
	~safe_bool();

	virtual bool operator_bool() const = 0;

private:
	safe_bool(const safe_bool& /*unused*/);
	safe_bool& operator=(const safe_bool& /*unused*/);
};

/**
 * To allow explicit comparison with bool.
 */
template <typename T>
bool operator==(const safe_bool<T>& lhs, bool b);
template <typename T>
bool operator==(bool b, const safe_bool<T>& rhs);
template <typename T>
bool operator!=(const safe_bool<T>& lhs, bool b);
template <typename T>
bool operator!=(bool b, const safe_bool<T>& rhs);

/**
 * To avoid unwanted comparisons with other types.
 */
template <typename T, typename U>
bool operator==(const safe_bool<T>& lhs, const U& /*rhs*/);
template <typename T, typename U>
bool operator==(const U& /*lhs*/, const safe_bool<T>& rhs);
template <typename T, typename U>
bool operator!=(const safe_bool<T>& lhs, const U& /*rhs*/);
template <typename T, typename U>
bool operator!=(const U& /*lhs*/, const safe_bool<T>& rhs);

} // namespace ft

#	include "libftpp/safe_bool/safe_bool.tpp"      // IWYU pragma: export
#	include "libftpp/safe_bool/safe_bool_void.tpp" // IWYU pragma: export

#endif // LIBFTPP_SAFE_BOOL_HPP
