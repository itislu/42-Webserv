// IWYU pragma: private; include "libftpp/iterator.hpp"
#pragma once
#ifndef LIBFTPP_ITERATOR_DATA_TPP
#	define LIBFTPP_ITERATOR_DATA_TPP

#	include "libftpp/iterator.hpp"
#	include <cstddef>

namespace ft {

template <typename C>
typename C::pointer data(C& c)
{
	return c.data();
}

template <typename C>
typename C::const_pointer data(const C& c)
{
	return c.data();
}

template <typename T, std::size_t N>
T* data(T (&array)[N]) throw()
{
	return array;
}

} // namespace ft

#endif // LIBFTPP_ITERATOR_DATA_TPP
