// IWYU pragma: private; include "libftpp/iterator.hpp"
#pragma once
#ifndef LIBFTPP_ITERATOR_ADVANCE_TPP
#	define LIBFTPP_ITERATOR_ADVANCE_TPP

#	include "libftpp/iterator.hpp"
#	include "libftpp/type_traits.hpp"
#	include <cassert>
#	include <cstdlib>
#	include <iterator>

namespace ft {

namespace _advance {
template <typename InputIt, typename Sentinel>
void advance(InputIt& it, Sentinel bound, std::input_iterator_tag /*unused*/);
template <typename InputIt, typename Sentinel>
void advance(InputIt& it,
             Sentinel bound,
             std::random_access_iterator_tag /*unused*/);
template <typename InputIt>
typename std::iterator_traits<InputIt>::difference_type
advance(InputIt& it,
        typename std::iterator_traits<InputIt>::difference_type n,
        InputIt bound,
        std::input_iterator_tag /*unused*/);
template <typename BidirIt>
typename std::iterator_traits<BidirIt>::difference_type
advance(BidirIt& it,
        typename std::iterator_traits<BidirIt>::difference_type n,
        BidirIt bound,
        std::bidirectional_iterator_tag /*unused*/);
template <typename RandomIt>
typename std::iterator_traits<RandomIt>::difference_type
advance(RandomIt& it,
        typename std::iterator_traits<RandomIt>::difference_type n,
        RandomIt bound,
        std::random_access_iterator_tag /*unused*/);
} // namespace _advance

template <typename InputIt>
void advance(InputIt& it,
             typename std::iterator_traits<InputIt>::difference_type n)
{
	std::advance(it, n);
}

template <typename InputIt, typename Sentinel>
FT_REQUIRES((!ft::is_convertible<
             Sentinel,
             typename std::iterator_traits<InputIt>::difference_type>::value))
(void)advance(InputIt& it, Sentinel bound)
{
	_advance::advance(
	    it, bound, typename std::iterator_traits<InputIt>::iterator_category());
}

template <typename InputIt, typename Sentinel>
typename std::iterator_traits<InputIt>::difference_type
advance(InputIt& it,
        typename std::iterator_traits<InputIt>::difference_type n,
        Sentinel bound)
{
	return _advance::advance(
	    it,
	    n,
	    bound,
	    typename std::iterator_traits<InputIt>::iterator_category());
}

namespace _advance {

template <typename InputIt, typename Sentinel>
void advance(InputIt& it, Sentinel bound, std::input_iterator_tag /*unused*/)
{
	while (it != bound) {
		++it;
	}
}

template <typename InputIt, typename Sentinel>
void
advance(InputIt& it, Sentinel bound, std::random_access_iterator_tag /*unused*/)
{
	ft::advance(it, bound - it);
}

template <typename InputIt>
typename std::iterator_traits<InputIt>::difference_type
advance(InputIt& it,
        typename std::iterator_traits<InputIt>::difference_type n,
        InputIt bound,
        std::input_iterator_tag /*unused*/)
{
	assert(n >= 0);
	while (n > 0 && it != bound) {
		++it;
		--n;
	}
	return n;
}

template <typename BidirIt>
typename std::iterator_traits<BidirIt>::difference_type
advance(BidirIt& it,
        typename std::iterator_traits<BidirIt>::difference_type n,
        BidirIt bound,
        std::bidirectional_iterator_tag /*unused*/)
{
	while (n > 0 && it != bound) {
		++it;
		--n;
	}
	while (n < 0 && it != bound) {
		--it;
		++n;
	}
	return n;
}

template <typename RandomIt>
typename std::iterator_traits<RandomIt>::difference_type
advance(RandomIt& it,
        typename std::iterator_traits<RandomIt>::difference_type n,
        RandomIt bound,
        std::random_access_iterator_tag /*unused*/)
{
	const typename std::iterator_traits<RandomIt>::difference_type dist =
	    bound - it;
	if (std::abs(n) >= std::abs(dist)) {
		ft::advance(it, bound);
		return n - dist;
	}
	ft::advance(it, n);
	return 0;
}

} // namespace _advance

} // namespace ft

#endif // LIBFTPP_ITERATOR_ADVANCE_TPP
