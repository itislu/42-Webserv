// IWYU pragma: private; include "libftpp/array.hpp"
#pragma once
#ifndef LIBFTPP_ARRAY_ARRAY_TPP
#	define LIBFTPP_ARRAY_ARRAY_TPP

#	include "libftpp/array.hpp"
#	include "libftpp/algorithm.hpp"
#	include "libftpp/assert.hpp"
#	include "libftpp/string.hpp"
#	include "libftpp/type_traits.hpp"
#	include <algorithm>
#	include <cassert>
#	include <cstddef>
#	include <stdexcept>

namespace ft {

namespace _array {

template <typename T, std::size_t N>
T& impl<T, N>::ref(const Type& t, std::size_t n) throw()
{
	FT_STATIC_ASSERT(N > 0); // Cannot index into a zero-sized array.
	/*
	The const_cast only removes const-ness temporarily added by the parameter
	type, not from truly const objects. When called from a const member
	function, the result is implicitly converted back to const before exposure
	to the caller.
	*/
	return const_cast<T&>(t[n]);
}

template <typename T, std::size_t N>
T* impl<T, N>::ptr(const Type& t) throw()
{
	if (N == 0) {
		return NULL;
	}
	/*
	The const_cast only removes const-ness temporarily added by the parameter
	type, not from truly const objects. When called from a const member
	function, the result is implicitly converted back to const before exposure
	to the caller.
	*/
	return const_cast<T*>(t); // NOLINT(cppcoreguidelines-pro-type-const-cast)
}

} // namespace _array

/* Element access */

template <typename T, std::size_t N>
typename array<T, N>::reference array<T, N>::at(size_type pos)
{
	if (pos >= N) {
		throw std::out_of_range("out-of-bounds access in ft::array: index ("
		                        + ft::to_string(pos) + ") >= array size ("
		                        + ft::to_string(N) + ")");
	}
	return _array::impl<T, FT_MAX(N, 1)>::ref(_elems, pos);
}

template <typename T, std::size_t N>
typename array<T, N>::const_reference array<T, N>::at(size_type pos) const
{
	if (pos >= N) {
		throw std::out_of_range("out-of-bounds access in ft::array: index ("
		                        + ft::to_string(pos) + ") >= array size ("
		                        + ft::to_string(N) + ")");
	}
	return _array::impl<T, FT_MAX(N, 1)>::ref(_elems, pos);
}

template <typename T, std::size_t N>
typename array<T, N>::reference array<T, N>::operator[](size_type pos)
{
	assert(pos < N);
	return _array::impl<T, N>::ref(_elems, pos);
}

template <typename T, std::size_t N>
typename array<T, N>::const_reference
array<T, N>::operator[](size_type pos) const
{
	assert(pos < N);
	return _array::impl<T, N>::ref(_elems, pos);
}

template <typename T, std::size_t N>
typename array<T, N>::reference array<T, N>::front()
{
	return _array::impl<T, N>::ref(_elems, 0);
}

template <typename T, std::size_t N>
typename array<T, N>::const_reference array<T, N>::front() const
{
	return _array::impl<T, N>::ref(_elems, 0);
}

template <typename T, std::size_t N>
typename array<T, N>::reference array<T, N>::back()
{
	return N > 0 ? _array::impl<T, N>::ref(_elems, N - 1)
	             : _array::impl<T, N>::ref(_elems, 0);
}

template <typename T, std::size_t N>
typename array<T, N>::const_reference array<T, N>::back() const
{
	return N > 0 ? _array::impl<T, N>::ref(_elems, N - 1)
	             : _array::impl<T, N>::ref(_elems, 0);
}

template <typename T, std::size_t N>
T* array<T, N>::data() throw()
{
	return _array::impl<T, N>::ptr(_elems);
}

template <typename T, std::size_t N>
const T* array<T, N>::data() const throw()
{
	return _array::impl<T, N>::ptr(_elems);
}

/* Iterators */

template <typename T, std::size_t N>
typename array<T, N>::iterator array<T, N>::begin() throw()
{
	return iterator(data());
}

template <typename T, std::size_t N>
typename array<T, N>::const_iterator array<T, N>::begin() const throw()
{
	return const_iterator(data());
}

template <typename T, std::size_t N>
typename array<T, N>::const_iterator array<T, N>::cbegin() const throw()
{
	return const_iterator(data());
}

template <typename T, std::size_t N>
typename array<T, N>::iterator array<T, N>::end() throw()
{
	return iterator(data() + N);
}

template <typename T, std::size_t N>
typename array<T, N>::const_iterator array<T, N>::end() const throw()
{
	return const_iterator(data() + N);
}

template <typename T, std::size_t N>
typename array<T, N>::const_iterator array<T, N>::cend() const throw()
{
	return const_iterator(data() + N);
}

template <typename T, std::size_t N>
typename array<T, N>::reverse_iterator array<T, N>::rbegin() throw()
{
	return reverse_iterator(end());
}

template <typename T, std::size_t N>
typename array<T, N>::const_reverse_iterator array<T, N>::rbegin() const throw()
{
	return const_reverse_iterator(end());
}

template <typename T, std::size_t N>
typename array<T, N>::const_reverse_iterator array<T, N>::crbegin() const
    throw()
{
	return const_reverse_iterator(end());
}

template <typename T, std::size_t N>
typename array<T, N>::reverse_iterator array<T, N>::rend() throw()
{
	return reverse_iterator(begin());
}

template <typename T, std::size_t N>
typename array<T, N>::const_reverse_iterator array<T, N>::rend() const throw()
{
	return const_reverse_iterator(begin());
}

template <typename T, std::size_t N>
typename array<T, N>::const_reverse_iterator array<T, N>::crend() const throw()
{
	return const_reverse_iterator(begin());
}

/* Capacity */

template <typename T, std::size_t N>
bool array<T, N>::empty() const throw()
{
	return size() == 0;
}

template <typename T, std::size_t N>
typename array<T, N>::size_type array<T, N>::size() const throw()
{
	return N;
}

template <typename T, std::size_t N>
typename array<T, N>::size_type array<T, N>::max_size() const throw()
{
	return N;
}

/* Operations */

template <typename T, std::size_t N>
void array<T, N>::fill(const T& value)
{
	std::fill(begin(), end(), value);
}

template <typename T, std::size_t N>
void array<T, N>::swap(array& other)
{
	std::swap_ranges(begin(), end(), other.begin());
}

/* Non-member functions */

template <typename T, std::size_t N>
bool operator==(const array<T, N>& lhs, const array<T, N>& rhs)
{
	return ft::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, std::size_t N>
bool operator!=(const array<T, N>& lhs, const array<T, N>& rhs)
{
	return !(lhs == rhs);
}

template <typename T, std::size_t N>
bool operator<(const array<T, N>& lhs, const array<T, N>& rhs)
{
	return std::lexicographical_compare(
	    lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, std::size_t N>
bool operator<=(const array<T, N>& lhs, const array<T, N>& rhs)
{
	return !(lhs > rhs);
}

template <typename T, std::size_t N>
bool operator>(const array<T, N>& lhs, const array<T, N>& rhs)
{
	return rhs < lhs;
}

template <typename T, std::size_t N>
bool operator>=(const array<T, N>& lhs, const array<T, N>& rhs)
{
	return !(lhs < rhs);
}

template <std::size_t I, typename T, std::size_t N>
T& get(array<T, N>& a)
{
	FT_STATIC_ASSERT(I < N); // Index out of bounds.
	return a[I];
}

template <std::size_t I, typename T, std::size_t N>
const T& get(const array<T, N>& a)
{
	FT_STATIC_ASSERT(I < N); // Index out of bounds.
	return a[I];
}

template <typename T, std::size_t N>
void swap(array<T, N>& lhs, array<T, N>& rhs)
{
	lhs.swap(rhs);
}

template <typename T, std::size_t N>
array<typename ft::remove_cv<T>::type, N> to_array(T (&a)[N])
{
	array<typename ft::remove_cv<T>::type, N> arr;
	std::copy(a, a + N, arr.begin());
	return arr;
}

} // namespace ft

#endif // LIBFTPP_ARRAY_ARRAY_TPP
