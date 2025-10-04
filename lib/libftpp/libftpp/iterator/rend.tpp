// IWYU pragma: private; include "libftpp/iterator.hpp"
#pragma once
#ifndef LIBFTPP_ITERATOR_REND_TPP
#	define LIBFTPP_ITERATOR_REND_TPP

#	include "libftpp/iterator.hpp"
#	include <cstddef>
#	include <iterator>

namespace ft {

template <typename C>
typename C::reverse_iterator rend(C& c)
{
	return c.rend();
}

template <typename C>
typename C::const_reverse_iterator rend(const C& c)
{
	return c.rend();
}

template <typename T, std::size_t N>
std::reverse_iterator<T*> rend(T (&array)[N])
{
	return std::reverse_iterator<T*>(array);
}

template <typename C>
typename C::const_reverse_iterator crend(const C& c)
{
	return ft::rend(c);
}

template <typename T, std::size_t N>
std::reverse_iterator<const T*> crend(const T (&array)[N])
{
	return ft::rend(array);
}

} // namespace ft

#endif // LIBFTPP_ITERATOR_REND_TPP
