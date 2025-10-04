// IWYU pragma: private; include "libftpp/iterator.hpp"
#pragma once

#include "libftpp/iterator.hpp"
#include <cstddef>

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
	return array;
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
