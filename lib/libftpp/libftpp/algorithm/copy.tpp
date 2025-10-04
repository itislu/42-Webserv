// IWYU pragma: private; include "libftpp/algorithm.hpp"
#pragma once
#ifndef LIBFTPP_ALGORITHM_COPY_TPP
#	define LIBFTPP_ALGORITHM_COPY_TPP

#	include "libftpp/algorithm.hpp"
#	include <algorithm>
#	include <iterator>

namespace ft {

namespace _copy {
template <typename InputIt, typename Size, typename OutputIt>
OutputIt copy_n(InputIt first,
                Size count,
                OutputIt result,
                std::input_iterator_tag /*unused*/);
template <typename InputIt, typename Size, typename OutputIt>
OutputIt copy_n(InputIt first,
                Size count,
                OutputIt result,
                std::random_access_iterator_tag /*unused*/);
} // namespace _copy

template <typename InputIt, typename OutputIt, typename UnaryPred>
OutputIt copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPred pred)
{
	for (; first != last; ++first) {
		if (pred(*first)) {
			*d_first = *first;
			++d_first;
		}
	}
	return d_first;
}

template <typename InputIt, typename Size, typename OutputIt>
OutputIt copy_n(InputIt first, Size count, OutputIt result)
{
	return _copy::copy_n(
	    first,
	    count,
	    result,
	    typename std::iterator_traits<InputIt>::iterator_category());
}

namespace _copy {

template <typename InputIt, typename Size, typename OutputIt>
OutputIt copy_n(InputIt first,
                Size count,
                OutputIt result,
                std::input_iterator_tag /*unused*/)
{
	for (Size i = 0; i < count; ++result, ++first, ++i) {
		*result = *first;
	}
	return result;
}

/**
 * Use `std::copy` since it is more optimized.
 */
template <typename InputIt, typename Size, typename OutputIt>
OutputIt copy_n(InputIt first,
                Size count,
                OutputIt result,
                std::random_access_iterator_tag /*unused*/)
{
	return std::copy(first, first + count, result);
}

} // namespace _copy

} // namespace ft

#endif // LIBFTPP_ALGORITHM_COPY_TPP
