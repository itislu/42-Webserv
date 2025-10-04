// IWYU pragma: private; include "libftpp/algorithm.hpp"
#pragma once

#include "libftpp/algorithm.hpp"
#include "libftpp/functional.hpp"

namespace ft {

template <typename ForwardIt>
bool is_sorted(ForwardIt first, ForwardIt last)
{
	return ft::is_sorted_until(first, last) == last;
}

template <typename ForwardIt, typename Compare>
bool is_sorted(ForwardIt first, ForwardIt last, Compare comp)
{
	return ft::is_sorted_until(first, last, comp) == last;
}

template <typename ForwardIt>
ForwardIt is_sorted_until(ForwardIt first, ForwardIt last)
{
	return ft::is_sorted_until(first, last, ft::less<>());
}

template <typename ForwardIt, typename Compare>
ForwardIt is_sorted_until(ForwardIt first, ForwardIt last, Compare comp)
{
	if (first == last) {
		return last;
	}
	for (ForwardIt next = first; ++next != last; first = next) {
		if (comp(*next, *first)) {
			return next;
		}
	}
	return last;
}

} // namespace ft
