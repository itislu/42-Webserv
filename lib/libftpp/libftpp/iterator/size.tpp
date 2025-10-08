// IWYU pragma: private; include "libftpp/iterator.hpp"
#pragma once
#ifndef LIBFTPP_ITERATOR_SIZE_TPP
#	define LIBFTPP_ITERATOR_SIZE_TPP

#	include "libftpp/iterator.hpp"
#	include <cstddef>

namespace ft {

template <typename C>
typename C::size_type size(const C& c)
{
	return c.size();
}

template <typename C>
typename C::difference_type ssize(const C& c)
{
	return static_cast<typename C::difference_type>(c.size());
}

template <typename T, std::size_t N>
std::size_t size(const T (& /*unused*/)[N]) throw()
{
	return N;
}

template <typename T, std::ptrdiff_t N>
std::ptrdiff_t ssize(const T (& /*unused*/)[N]) throw()
{
	return N;
}

} // namespace ft

#endif // LIBFTPP_ITERATOR_SIZE_TPP
