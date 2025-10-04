// IWYU pragma: private; include "libftpp/iterator.hpp"
#pragma once

#include "libftpp/iterator.hpp"
#include <cstddef>

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
	return array + N;
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
