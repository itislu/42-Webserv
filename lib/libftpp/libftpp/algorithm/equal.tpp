// IWYU pragma: private; include "libftpp/algorithm.hpp"
#pragma once
#ifndef LIBFTPP_ALGORITHM_EQUAL_TPP
#	define LIBFTPP_ALGORITHM_EQUAL_TPP

#	include "libftpp/algorithm.hpp"
#	include <algorithm>

namespace ft {

template <typename InputIt1, typename InputIt2>
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2)
{
	return std::equal(first1, last1, first2);
}

template <typename InputIt1, typename InputIt2, typename BinaryPred>
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPred p)
{
	return std::equal(first1, last1, first2, p);
}

template <typename InputIt1, typename InputIt2>
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
{
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (!(*first1 == *first2)) {
			return false;
		}
	}
	return first1 == last1 && first2 == last2;
}

template <typename InputIt1, typename InputIt2, typename BinaryPred>
bool equal(InputIt1 first1,
           InputIt1 last1,
           InputIt2 first2,
           InputIt2 last2,
           BinaryPred p)
{
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (!p(*first1, *first2)) {
			return false;
		}
	}
	return first1 == last1 && first2 == last2;
}

} // namespace ft

#endif // LIBFTPP_ALGORITHM_EQUAL_TPP
