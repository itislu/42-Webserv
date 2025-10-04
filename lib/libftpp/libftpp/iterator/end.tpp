// IWYU pragma: private; include "libftpp/iterator.hpp"
#pragma once
#ifndef LIBFTPP_ITERATOR_END_TPP
#	define LIBFTPP_ITERATOR_END_TPP

#	include "libftpp/iterator.hpp"
#	include <cstddef>

namespace ft {

template <typename C>
typename C::iterator end(C& c)
{
	return c.end();
}

template <typename C>
typename C::const_iterator end(const C& c)
{
	return c.end();
}

template <typename T, std::size_t N>
T* end(T (&array)[N])
{
	// Get one past the end.
	// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
	return static_cast<T*>(array) + N;
}

template <typename C>
typename C::const_iterator cend(const C& c)
{
	return ft::end(c);
}

template <typename T, std::size_t N>
const T* cend(const T (&array)[N])
{
	return ft::end(array);
}

} // namespace ft

#endif // LIBFTPP_ITERATOR_END_TPP
