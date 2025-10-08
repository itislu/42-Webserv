// IWYU pragma: private; include "libftpp/algorithm.hpp"
#pragma once
#ifndef LIBFTPP_ALGORITHM_SHIFT_TPP
#	define LIBFTPP_ALGORITHM_SHIFT_TPP

#	include "libftpp/algorithm.hpp"
#	include "libftpp/iterator.hpp"
#	include <cassert>
#	include <iterator>

namespace ft {

namespace _shift {
template <typename ForwardIt>
ForwardIt
shift_right(ForwardIt first,
            ForwardIt last,
            typename std::iterator_traits<ForwardIt>::difference_type n,
            std::forward_iterator_tag /*unused*/);
template <typename BidirIt>
BidirIt shift_right(BidirIt first,
                    BidirIt last,
                    typename std::iterator_traits<BidirIt>::difference_type n,
                    std::bidirectional_iterator_tag /*unused*/);
} // namespace _shift

template <typename ForwardIt>
ForwardIt
shift_left(ForwardIt first,
           ForwardIt last,
           typename std::iterator_traits<ForwardIt>::difference_type n)
{
	assert(n >= 0);
	if (n == 0) {
		return last;
	}
	const ForwardIt mid = ft::next(first, n, last);
	if (mid == last) {
		return first;
	}
	return std::copy(mid, last, first);
}

template <typename ForwardIt>
ForwardIt
shift_right(ForwardIt first,
            ForwardIt last,
            typename std::iterator_traits<ForwardIt>::difference_type n)
{
	assert(n >= 0);
	if (n == 0) {
		return first;
	}
	return _shift::shift_right(
	    first,
	    last,
	    n,
	    typename std::iterator_traits<ForwardIt>::iterator_category());
}

namespace _shift {

template <typename ForwardIt>
ForwardIt
shift_right(ForwardIt first,
            ForwardIt last,
            typename std::iterator_traits<ForwardIt>::difference_type n,
            std::forward_iterator_tag /*unused*/)
{
	const ForwardIt dest_begin = ft::next(first, n, last);
	if (dest_begin == last) {
		return last;
	}

	typename std::iterator_traits<ForwardIt>::difference_type dest_size =
	    std::distance(dest_begin, last);
	if (dest_size <= n) {
		// When there is no overlap between source and dest range, no need to
		// preserve values in dest.
		ft::copy_n(first, dest_size, dest_begin);
		return dest_begin;
	}

	// Swap for overlapping source and dest range.
	ForwardIt dest = dest_begin;
	ForwardIt buf = first; // Circular swap space ending at `dest_begin`.
	for (; dest_size > n; --dest_size) {
		std::iter_swap(buf, dest);
		++buf;
		++dest;
		if (buf == dest_begin) {
			buf = first;
		}
	}
	// Remaining dest values were not in source range, so can be overwritten.
	dest = std::copy(buf, dest_begin, dest);
	std::copy(first, buf, dest);
	return dest_begin;
}

template <typename BidirIt>
BidirIt shift_right(BidirIt first,
                    BidirIt last,
                    typename std::iterator_traits<BidirIt>::difference_type n,
                    std::bidirectional_iterator_tag /*unused*/)
{
	const BidirIt mid = ft::next(last, -n, first);
	if (mid == first) {
		return last;
	}
	return std::copy_backward(first, mid, last);
}

} // namespace _shift

} // namespace ft

#endif // LIBFTPP_ALGORITHM_SHIFT_TPP
