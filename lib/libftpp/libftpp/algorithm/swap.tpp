// IWYU pragma: private; include "libftpp/algorithm.hpp"
#pragma once
#ifndef LIBFTPP_ALGORITHM_SWAP_TPP
#	define LIBFTPP_ALGORITHM_SWAP_TPP

#	include "libftpp/algorithm.hpp"
#	include "libftpp/type_traits.hpp"
#	include <algorithm>
#	include <cstddef>

namespace ft {

namespace _swap {
template <typename T>
FT_REQUIRES(ft::has_member_function_swap<T>::value)
(void)member_swap(T& a, T& b);
template <typename T>
FT_REQUIRES(!ft::has_member_function_swap<T>::value)
(void)member_swap(T& a, T& b);
} // namespace _swap

template <typename T>
void member_swap(T& a, T& b)
{
	_swap::member_swap(a, b);
}

template <typename T2, std::size_t N>
void member_swap(T2 (&a)[N], T2 (&b)[N])
{
	ft::swap_ranges(a, a + N, b);
}

template <typename ForwardIt1, typename ForwardIt2>
void iter_swap(ForwardIt1 a, ForwardIt2 b)
{
	ft::member_swap(*a, *b);
}

template <typename ForwardIt1, typename ForwardIt2>
ForwardIt2 swap_ranges(ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2)
{
	for (; first1 != last1; ++first1, ++first2) {
		ft::iter_swap(first1, first2);
	}
	return first2;
}

namespace _swap {

/**
 * Use the type's `swap` member function.
 */
template <typename T>
FT_REQUIRES(ft::has_member_function_swap<T>::value)
(void)member_swap(T& a, T& b) { a.swap(b); }

/**
 * Fall back to the common swap pattern for types that do not have a `swap`
 * member function.
 */
template <typename T>
FT_REQUIRES(!ft::has_member_function_swap<T>::value)
(void)member_swap(T& a, T& b)
{
	using std::swap;
	swap(a, b);
}

} // namespace _swap

} // namespace ft

#endif // LIBFTPP_ALGORITHM_SWAP_TPP
