// IWYU pragma: private; include "libftpp/iterator.hpp"
#pragma once
#ifndef LIBFTPP_ITERATOR_NEXT_TPP
#	define LIBFTPP_ITERATOR_NEXT_TPP

#	include "libftpp/assert.hpp"
#	include "libftpp/iterator.hpp"
#	include "libftpp/type_traits.hpp"
#	include <iterator>

namespace ft {

template <typename InputIt>
InputIt next(InputIt it)
{
	typedef typename std::iterator_traits<InputIt>::difference_type
	    InputIt_must_be_an_iterator_type;
	// Use to avoid unused typedef warnings.
	FT_STATIC_ASSERT(ft::make_true<InputIt_must_be_an_iterator_type>::value);

	++it;
	return it;
}

template <typename InputIt>
InputIt next(InputIt it,
             typename std::iterator_traits<InputIt>::difference_type n)
{
	ft::advance(it, n);
	return it;
}

template <typename InputIt, typename Sentinel>
FT_REQUIRES((!ft::is_convertible<
             Sentinel,
             typename std::iterator_traits<InputIt>::difference_type>::value))
(InputIt) next(InputIt it, Sentinel bound)
{
	ft::advance(it, bound);
	return it;
}

template <typename InputIt, typename Sentinel>
InputIt next(InputIt it,
             typename std::iterator_traits<InputIt>::difference_type n,
             Sentinel bound)
{
	ft::advance(it, n, bound);
	return it;
}

} // namespace ft

#endif // LIBFTPP_ITERATOR_NEXT_TPP
