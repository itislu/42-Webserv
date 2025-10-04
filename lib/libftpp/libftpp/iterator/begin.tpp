// IWYU pragma: private; include "libftpp/iterator.hpp"
#pragma once
#ifndef LIBFTPP_ITERATOR_BEGIN_TPP
#	define LIBFTPP_ITERATOR_BEGIN_TPP

#	include "libftpp/iterator.hpp"
#	include <cstddef>

namespace ft {

template <typename C>
typename C::iterator begin(C& c)
{
	return c.begin();
}

template <typename C>
typename C::const_iterator begin(const C& c)
{
	return c.begin();
}

template <typename T, std::size_t N>
T* begin(T (&array)[N])
{
	return static_cast<T*>(array);
}

template <typename C>
typename C::const_iterator cbegin(const C& c)
{
	return ft::begin(c);
}

template <typename T, std::size_t N>
const T* cbegin(const T (&array)[N])
{
	return ft::begin(array);
}

} // namespace ft

#endif // LIBFTPP_ITERATOR_BEGIN_TPP
