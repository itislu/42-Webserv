// IWYU pragma: private; include "libftpp/algorithm.hpp"
#pragma once
#ifndef LIBFTPP_ALGORITHM_BINARY_SEARCH_TPP
#	define LIBFTPP_ALGORITHM_BINARY_SEARCH_TPP

#	include "libftpp/algorithm.hpp"
#	include "libftpp/functional.hpp"
#	include "libftpp/iterator.hpp"
#	include <iterator>
#	include <utility>

namespace ft {

/* lower_bound */

template <typename ForwardIt, typename T>
ForwardIt
lower_bound(ForwardIt first,
            typename std::iterator_traits<ForwardIt>::difference_type count,
            const T& value)
{
	return ft::lower_bound(first, count, value, ft::less<>());
}

template <typename ForwardIt, typename T, typename Compare>
ForwardIt
lower_bound(ForwardIt first,
            typename std::iterator_traits<ForwardIt>::difference_type count,
            const T& value,
            Compare comp)
{
	ForwardIt mid;
	typename std::iterator_traits<ForwardIt>::difference_type half;

	while (count > 0) {
		half = count / 2;
		mid = ft::next(first, half);

		if (comp(*mid, value)) {
			first = ++mid;
			count -= half + 1;
		}
		else {
			count = half;
		}
	}
	return first;
}

/* upper_bound */

template <typename ForwardIt, typename T>
ForwardIt
upper_bound(ForwardIt first,
            typename std::iterator_traits<ForwardIt>::difference_type count,
            const T& value)
{
	return ft::upper_bound(first, count, value, ft::less<>());
}

template <typename ForwardIt, typename T, typename Compare>
ForwardIt
upper_bound(ForwardIt first,
            typename std::iterator_traits<ForwardIt>::difference_type count,
            const T& value,
            Compare comp)
{
	ForwardIt mid;
	typename std::iterator_traits<ForwardIt>::difference_type half;

	while (count > 0) {
		half = count / 2;
		mid = ft::next(first, half);

		if (!comp(value, *mid)) {
			first = ++mid;
			count -= half + 1;
		}
		else {
			count = half;
		}
	}
	return first;
}

/* equal_range */

template <typename ForwardIt, typename T>
std::pair<ForwardIt, ForwardIt>
equal_range(ForwardIt first,
            typename std::iterator_traits<ForwardIt>::difference_type count,
            const T& value)
{
	return ft::equal_range(first, count, value, ft::less<>());
}

template <typename ForwardIt, typename T, typename Compare>
std::pair<ForwardIt, ForwardIt>
equal_range(ForwardIt first,
            typename std::iterator_traits<ForwardIt>::difference_type count,
            const T& value,
            Compare comp)
{
	ForwardIt mid;
	typename std::iterator_traits<ForwardIt>::difference_type half;

	while (count > 0) {
		half = count / 2;
		mid = ft::next(first, half);

		if (comp(*mid, value)) {
			first = ++mid;
			count -= half + 1;
		}
		else if (comp(value, *mid)) {
			count = half;
		}
		else {
			return std::make_pair(
			    ft::lower_bound(first, half, value, comp),
			    ft::upper_bound(++mid, count - (half + 1), value, comp));
		}
	}
	return std::make_pair(first, first);
}

/* binary_search */

template <typename ForwardIt, typename T>
bool
binary_search(ForwardIt first,
              typename std::iterator_traits<ForwardIt>::difference_type count,
              const T& value)
{
	return ft::binary_search(first, count, value, ft::less<>());
}

template <typename ForwardIt, typename T, typename Compare>
bool
binary_search(ForwardIt first,
              typename std::iterator_traits<ForwardIt>::difference_type count,
              const T& value,
              Compare comp)
{
	ForwardIt mid;
	typename std::iterator_traits<ForwardIt>::difference_type half;
	bool not_last = false;

	while (count > 0) {
		half = count / 2;
		mid = ft::next(first, half);

		if (comp(*mid, value)) {
			first = ++mid;
			count -= half + 1;
		}
		else {
			count = half;
			not_last = true;
		}
	}
	return not_last && !comp(value, *first);
}

} // namespace ft

#endif // LIBFTPP_ALGORITHM_BINARY_SEARCH_TPP
