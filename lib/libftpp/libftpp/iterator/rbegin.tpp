// IWYU pragma: private; include "libftpp/iterator.hpp"
#pragma once
#ifndef LIBFTPP_ITERATOR_RBEGIN_TPP
#	define LIBFTPP_ITERATOR_RBEGIN_TPP

#	include "libftpp/iterator.hpp"
#	include <cstddef>
#	include <iterator>

namespace ft {

template <typename C>
typename C::reverse_iterator rbegin(C& c)
{
	return c.rbegin();
}

template <typename C>
typename C::const_reverse_iterator rbegin(const C& c)
{
	return c.rbegin();
}

template <typename T, std::size_t N>
std::reverse_iterator<T*> rbegin(T (&array)[N])
{
	return std::reverse_iterator<T*>(array + N);
}

template <typename C>
typename C::const_reverse_iterator crbegin(const C& c)
{
	return ft::rbegin(c);
}

template <typename T, std::size_t N>
std::reverse_iterator<const T*> crbegin(const T (&array)[N])
{
	return ft::rbegin(array);
}

} // namespace ft

#endif // LIBFTPP_ITERATOR_RBEGIN_TPP
