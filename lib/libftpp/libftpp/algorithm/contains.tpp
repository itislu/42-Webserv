// IWYU pragma: private; include "libftpp/algorithm.hpp"
#pragma once
#ifndef LIBFTPP_ALGORITHM_CONTAINS_TPP
#	define LIBFTPP_ALGORITHM_CONTAINS_TPP

#	include "libftpp/algorithm.hpp"
#	include "libftpp/iterator.hpp"
#	include <algorithm>

namespace ft {

template <typename InputIt, typename T>
bool contains(InputIt first, InputIt last, const T& value)
{
	return std::find(first, last, value) != last;
}

template <typename R, typename T>
bool contains(const R& r, const T& value)
{
	return ft::contains(ft::begin(r), ft::end(r), value);
}

template <typename InputIt1, typename InputIt2>
bool contains_subrange(InputIt1 first1,
                       InputIt1 last1,
                       InputIt2 first2,
                       InputIt2 last2)
{
	return first2 == last2
	       || std::search(first1, last1, first2, last2) != last1;
}

template <typename R1, typename R2>
bool contains_subrange(const R1& r1, const R2& r2)
{
	return ft::contains_subrange(
	    ft::begin(r1), ft::end(r1), ft::begin(r2), ft::end(r2));
}

} // namespace ft

#endif // LIBFTPP_ALGORITHM_CONTAINS_TPP
