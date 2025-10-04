// IWYU pragma: private; include "libftpp/algorithm.hpp"
#pragma once
#ifndef LIBFTPP_ALGORITHM_MIN_MAX_TPP
#	define LIBFTPP_ALGORITHM_MIN_MAX_TPP

#	include "libftpp/algorithm.hpp"

namespace ft {

template <typename T>
T& max(T& a, T& b)
{
	return (a < b) ? b : a;
}

template <typename T, typename Compare>
T& max(T& a, T& b, Compare comp)
{
	return comp(a, b) ? b : a;
}

template <typename T>
const T& max(const T& a, const T& b)
{
	// NOLINTNEXTLINE(bugprone-return-const-ref-from-parameter): Same as stdlib.
	return (a < b) ? b : a;
}

template <typename T, typename Compare>
const T& max(const T& a, const T& b, Compare comp)
{
	// NOLINTNEXTLINE(bugprone-return-const-ref-from-parameter): Same as stdlib.
	return comp(a, b) ? b : a;
}

template <typename T>
T& min(T& a, T& b)
{
	return (b < a) ? b : a;
}

template <typename T, typename Compare>
T& min(T& a, T& b, Compare comp)
{
	return comp(b, a) ? b : a;
}

template <typename T>
const T& min(const T& a, const T& b)
{
	// NOLINTNEXTLINE(bugprone-return-const-ref-from-parameter): Same as stdlib.
	return (b < a) ? b : a;
}

template <typename T, typename Compare>
const T& min(const T& a, const T& b, Compare comp)
{
	// NOLINTNEXTLINE(bugprone-return-const-ref-from-parameter): Same as stdlib.
	return comp(b, a) ? b : a;
}

} // namespace ft

#endif // LIBFTPP_ALGORITHM_MIN_MAX_TPP
