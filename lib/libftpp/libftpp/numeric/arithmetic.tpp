// IWYU pragma: private; include "libftpp/numeric.hpp"
#pragma once
#ifndef LIBFTPP_NUMERIC_ARITHMETIC_TPP
#	define LIBFTPP_NUMERIC_ARITHMETIC_TPP

#	include "libftpp/expected.hpp"
#	include "libftpp/numeric.hpp"
#	include "libftpp/type_traits.hpp"
#	include "libftpp/utility.hpp"
#	include <limits>

namespace ft {

namespace _arithmetic {
enum Error {
	OK,
	NEGATIVE_OVERFLOW,
	POSITIVE_OVERFLOW,
	DIVISION_BY_ZERO
};
template <typename T>
FT_REQUIRES(ft::is_integral<T>::value)
(Error) add(T x, T y, T* result) throw();
template <typename T>
FT_REQUIRES(ft::is_integral<T>::value)
(Error) sub(T x, T y, T* result) throw();
template <typename T>
FT_REQUIRES(ft::is_integral<T>::value)
(Error) mul(T x, T y, T* result) throw();
template <typename T>
FT_REQUIRES(ft::is_integral<T>::value)
(Error) div(T x, T y, T* result) throw();
} // namespace _arithmetic

/* Checked arithmetic */

template <typename T>
FT_REQUIRES(ft::is_integral<T>::value)
((ft::expected<T, arithmetic_exception>)) add_checked(T x, T y)
{
	T result;
	switch (_arithmetic::add(x, y, &result)) {
	case _arithmetic::OK:
		return result;
	case _arithmetic::NEGATIVE_OVERFLOW:
		return ft::unexpected<arithmetic_exception>(
		    arithmetic_negative_overflow_exception("ft::add_checked"));
	case _arithmetic::POSITIVE_OVERFLOW:
		return ft::unexpected<arithmetic_exception>(
		    arithmetic_positive_overflow_exception("ft::add_checked"));
	case _arithmetic::DIVISION_BY_ZERO:
		FT_UNREACHABLE();
	}
	FT_UNREACHABLE();
}

template <typename T>
FT_REQUIRES(ft::is_integral<T>::value)
((ft::expected<T, arithmetic_exception>)) sub_checked(T x, T y)
{
	T result;
	switch (_arithmetic::sub(x, y, &result)) {
	case _arithmetic::OK:
		return result;
	case _arithmetic::NEGATIVE_OVERFLOW:
		return ft::unexpected<arithmetic_exception>(
		    arithmetic_negative_overflow_exception("ft::sub_checked"));
	case _arithmetic::POSITIVE_OVERFLOW:
		return ft::unexpected<arithmetic_exception>(
		    arithmetic_positive_overflow_exception("ft::sub_checked"));
	case _arithmetic::DIVISION_BY_ZERO:
		FT_UNREACHABLE();
	}
	FT_UNREACHABLE();
}

template <typename T>
FT_REQUIRES(ft::is_integral<T>::value)
((ft::expected<T, arithmetic_exception>)) mul_checked(T x, T y)
{
	T result;
	switch (_arithmetic::mul(x, y, &result)) {
	case _arithmetic::OK:
		return result;
	case _arithmetic::NEGATIVE_OVERFLOW:
		return ft::unexpected<arithmetic_exception>(
		    arithmetic_negative_overflow_exception("ft::mul_checked"));
	case _arithmetic::POSITIVE_OVERFLOW:
		return ft::unexpected<arithmetic_exception>(
		    arithmetic_positive_overflow_exception("ft::mul_checked"));
	case _arithmetic::DIVISION_BY_ZERO:
		FT_UNREACHABLE();
	}
	FT_UNREACHABLE();
}

template <typename T>
FT_REQUIRES(ft::is_integral<T>::value)
((ft::expected<T, arithmetic_exception>)) div_checked(T x, T y)
{
	T result;
	switch (_arithmetic::div(x, y, &result)) {
	case _arithmetic::OK:
		return result;
	case _arithmetic::NEGATIVE_OVERFLOW:
		FT_UNREACHABLE();
	case _arithmetic::POSITIVE_OVERFLOW:
		return ft::unexpected<arithmetic_exception>(
		    arithmetic_positive_overflow_exception("ft::div_checked"));
	case _arithmetic::DIVISION_BY_ZERO:
		return ft::unexpected<arithmetic_exception>(
		    arithmetic_division_by_zero_exception("ft::div_checked"));
	}
	FT_UNREACHABLE();
}

/* Saturating arithmetic */

template <typename T>
FT_REQUIRES(ft::is_integral<T>::value)
(T) add_sat(T x, T y) throw()
{
	T result;
	switch (_arithmetic::add(x, y, &result)) {
	case _arithmetic::OK:
		return result;
	case _arithmetic::NEGATIVE_OVERFLOW:
		return std::numeric_limits<T>::min();
	case _arithmetic::POSITIVE_OVERFLOW:
		return std::numeric_limits<T>::max();
	case _arithmetic::DIVISION_BY_ZERO:
		FT_UNREACHABLE();
	}
	FT_UNREACHABLE();
}

template <typename T>
FT_REQUIRES(ft::is_integral<T>::value)
(T) sub_sat(T x, T y) throw()
{
	T result;
	switch (_arithmetic::sub(x, y, &result)) {
	case _arithmetic::OK:
		return result;
	case _arithmetic::NEGATIVE_OVERFLOW:
		return std::numeric_limits<T>::min();
	case _arithmetic::POSITIVE_OVERFLOW:
		return std::numeric_limits<T>::max();
	case _arithmetic::DIVISION_BY_ZERO:
		FT_UNREACHABLE();
	}
	FT_UNREACHABLE();
}

template <typename T>
FT_REQUIRES(ft::is_integral<T>::value)
(T) mul_sat(T x, T y) throw()
{
	T result;
	switch (_arithmetic::mul(x, y, &result)) {
	case _arithmetic::OK:
		return result;
	case _arithmetic::NEGATIVE_OVERFLOW:
		return std::numeric_limits<T>::min();
	case _arithmetic::POSITIVE_OVERFLOW:
		return std::numeric_limits<T>::max();
	case _arithmetic::DIVISION_BY_ZERO:
		FT_UNREACHABLE();
	}
	FT_UNREACHABLE();
}

template <typename T>
FT_REQUIRES(ft::is_integral<T>::value)
(T) div_sat(T x, T y) throw()
{
	T result;
	switch (_arithmetic::div(x, y, &result)) {
	case _arithmetic::OK:
		return result;
	case _arithmetic::NEGATIVE_OVERFLOW:
		FT_UNREACHABLE();
	case _arithmetic::POSITIVE_OVERFLOW:
		return std::numeric_limits<T>::max();
	case _arithmetic::DIVISION_BY_ZERO:
		if (x > 0) {
			return std::numeric_limits<T>::max();
		}
		if (x < 0) {
			return std::numeric_limits<T>::min();
		}
		return 0; // 0/0
	}
	FT_UNREACHABLE();
}

/* Throwing arithmetic */

template <typename T>
FT_REQUIRES(ft::is_integral<T>::value)
(T) add_throw(T x, T y)
{
	T result;
	switch (_arithmetic::add(x, y, &result)) {
	case _arithmetic::OK:
		return result;
	case _arithmetic::NEGATIVE_OVERFLOW:
		throw arithmetic_negative_overflow_exception("ft::add_throw");
	case _arithmetic::POSITIVE_OVERFLOW:
		throw arithmetic_positive_overflow_exception("ft::add_throw");
	case _arithmetic::DIVISION_BY_ZERO:
		FT_UNREACHABLE();
	}
	FT_UNREACHABLE();
}

template <typename T>
FT_REQUIRES(ft::is_integral<T>::value)
(T) sub_throw(T x, T y)
{
	T result;
	switch (_arithmetic::sub(x, y, &result)) {
	case _arithmetic::OK:
		return result;
	case _arithmetic::NEGATIVE_OVERFLOW:
		throw arithmetic_negative_overflow_exception("ft::sub_throw");
	case _arithmetic::POSITIVE_OVERFLOW:
		throw arithmetic_positive_overflow_exception("ft::sub_throw");
	case _arithmetic::DIVISION_BY_ZERO:
		FT_UNREACHABLE();
	}
	FT_UNREACHABLE();
}

template <typename T>
FT_REQUIRES(ft::is_integral<T>::value)
(T) mul_throw(T x, T y)
{
	T result;
	switch (_arithmetic::mul(x, y, &result)) {
	case _arithmetic::OK:
		return result;
	case _arithmetic::NEGATIVE_OVERFLOW:
		throw arithmetic_negative_overflow_exception("ft::mul_throw");
	case _arithmetic::POSITIVE_OVERFLOW:
		throw arithmetic_positive_overflow_exception("ft::mul_throw");
	case _arithmetic::DIVISION_BY_ZERO:
		FT_UNREACHABLE();
	}
	FT_UNREACHABLE();
}

template <typename T>
FT_REQUIRES(ft::is_integral<T>::value)
(T) div_throw(T x, T y)
{
	T result;
	switch (_arithmetic::div(x, y, &result)) {
	case _arithmetic::OK:
		return result;
	case _arithmetic::NEGATIVE_OVERFLOW:
		FT_UNREACHABLE();
	case _arithmetic::POSITIVE_OVERFLOW:
		throw arithmetic_positive_overflow_exception("ft::div_throw");
	case _arithmetic::DIVISION_BY_ZERO:
		throw arithmetic_division_by_zero_exception("ft::div_throw");
	}
	FT_UNREACHABLE();
}

namespace _arithmetic {

template <typename T>
FT_REQUIRES(ft::is_integral<T>::value)
(Error) add(T x, T y, T* result) throw()
{
	const T max = std::numeric_limits<T>::max();
	if (std::numeric_limits<T>::is_signed) {
		if (x > 0 && y > 0 && x > max - y) {
			return POSITIVE_OVERFLOW;
		}
		const T min = std::numeric_limits<T>::min();
		if (x < 0 && y < 0 && x < min - y) {
			return NEGATIVE_OVERFLOW;
		}
	}
	else {
		// For unsigned types only positive overflow is possible
		if (y > max - x) {
			return POSITIVE_OVERFLOW;
		}
	}
	*result = x + y;
	return OK;
}

template <typename T>
FT_REQUIRES(ft::is_integral<T>::value)
(Error) sub(T x, T y, T* result) throw()
{
	if (std::numeric_limits<T>::is_signed) {
		const T max = std::numeric_limits<T>::max();
		const T min = std::numeric_limits<T>::min();
		if (x > 0 && y < 0 && x > max + y) {
			return POSITIVE_OVERFLOW;
		}
		if (x < 0 && y > 0 && x < min + y) {
			return NEGATIVE_OVERFLOW;
		}
	}
	else {
		// For unsigned types only negative overflow is possible
		if (x < y) {
			return NEGATIVE_OVERFLOW;
		}
	}
	*result = x - y;
	return OK;
}

template <typename T>
FT_REQUIRES(ft::is_integral<T>::value)
(Error) mul(T x, T y, T* result) throw()
{
	const T max = std::numeric_limits<T>::max();
	if (std::numeric_limits<T>::is_signed) {
		if ((x > 0 && y > 0 && x > max / y)
		    || (x < 0 && y < 0 && x < max / y)) {
			return POSITIVE_OVERFLOW;
		}
		const T min = std::numeric_limits<T>::min();
		if ((x > 0 && y < 0 && y < min / x)
		    || (x < 0 && y > 0 && x < min / y)) {
			return NEGATIVE_OVERFLOW;
		}
	}
	else {
		// For unsigned types only positive overflow is possible
		if (y > 0 && x > max / y) {
			return POSITIVE_OVERFLOW;
		}
	}
	*result = x * y;
	return OK;
}

template <typename T>
FT_REQUIRES(ft::is_integral<T>::value)
(Error) div(T x, T y, T* result) throw()
{
	if (y == 0) {
		return DIVISION_BY_ZERO;
	}

	if (std::numeric_limits<T>::is_signed) {
		const T min = std::numeric_limits<T>::min();
		// MIN / -1 positive overflow
		if (x == min && y == -1) {
			return POSITIVE_OVERFLOW;
		}
	}
	*result = x / y;
	return OK;
}

} // namespace _arithmetic

} // namespace ft

#endif // LIBFTPP_NUMERIC_ARITHMETIC_TPP
