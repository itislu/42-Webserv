#pragma once
#ifndef LIBFTPP_ARRAY_HPP
#	define LIBFTPP_ARRAY_HPP

#	include "libftpp/algorithm.hpp"
#	include "libftpp/iterator/begin.tpp"  // IWYU pragma: export
#	include "libftpp/iterator/data.tpp"   // IWYU pragma: export
#	include "libftpp/iterator/empty.tpp"  // IWYU pragma: export
#	include "libftpp/iterator/end.tpp"    // IWYU pragma: export
#	include "libftpp/iterator/rbegin.tpp" // IWYU pragma: export
#	include "libftpp/iterator/rend.tpp"   // IWYU pragma: export
#	include "libftpp/iterator/size.tpp"   // IWYU pragma: export
#	include "libftpp/type_traits.hpp"
#	include <cstddef>
#	include <iterator>

namespace ft {

namespace _array {

template <typename T, std::size_t N>
struct impl {
	typedef T Type[FT_MAX(N, 1)];

	static T& ref(const Type& t, std::size_t n) throw();
	static T* ptr(const Type& t) throw();
};

} // namespace _array

/**
 * https://en.cppreference.com/w/cpp/container/array
 */
template <typename T, std::size_t N>
struct array {
	typedef T value_type;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type* iterator;
	typedef const value_type* const_iterator;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	// No explicit construct/copy/destroy for aggregate type.

	/* Element access */
	reference at(size_type pos);
	const_reference at(size_type pos) const;
	reference operator[](size_type pos);
	const_reference operator[](size_type pos) const;
	reference front();
	const_reference front() const;
	reference back();
	const_reference back() const;
	T* data() throw();
	const T* data() const throw();

	/* Iterators */
	iterator begin() throw();
	const_iterator begin() const throw();
	const_iterator cbegin() const throw();
	iterator end() throw();
	const_iterator end() const throw();
	const_iterator cend() const throw();
	reverse_iterator rbegin() throw();
	const_reverse_iterator rbegin() const throw();
	const_reverse_iterator crbegin() const throw();
	reverse_iterator rend() throw();
	const_reverse_iterator rend() const throw();
	const_reverse_iterator crend() const throw();

	/* Capacity */
	bool empty() const throw();
	size_type size() const throw();
	size_type max_size() const throw();

	/* Operations */
	void fill(const T& value);
	void swap(array& other);

	typename _array::impl<T, N>::Type
	    _elems; // NOLINT: Public to allow aggregate initialization
};

template <typename T, std::size_t N>
bool operator==(const array<T, N>& lhs, const array<T, N>& rhs);
template <typename T, std::size_t N>
bool operator!=(const array<T, N>& lhs, const array<T, N>& rhs);
template <typename T, std::size_t N>
bool operator<(const array<T, N>& lhs, const array<T, N>& rhs);
template <typename T, std::size_t N>
bool operator<=(const array<T, N>& lhs, const array<T, N>& rhs);
template <typename T, std::size_t N>
bool operator>(const array<T, N>& lhs, const array<T, N>& rhs);
template <typename T, std::size_t N>
bool operator>=(const array<T, N>& lhs, const array<T, N>& rhs);

template <std::size_t I, typename T, std::size_t N>
T& get(array<T, N>& a);
template <std::size_t I, typename T, std::size_t N>
const T& get(const array<T, N>& a);

template <typename T, std::size_t N>
void swap(array<T, N>& lhs, array<T, N>& rhs);

template <typename T, std::size_t N>
array<typename ft::remove_cv<T>::type, N> to_array(T (&a)[N]);

} // namespace ft

#	include "libftpp/array/array.tpp" // IWYU pragma: export

#endif // LIBFTPP_ARRAY_HPP
