#pragma once
#ifndef LIBFTPP_MEMORY_DEFAULT_DELETE_TPP
#	define LIBFTPP_MEMORY_DEFAULT_DELETE_TPP

#	include "libftpp/memory.hpp"
#	include "libftpp/assert.hpp"
#	include "libftpp/type_traits.hpp"

namespace ft {

/* default_delete<T> */

template <typename T>
default_delete<T>::default_delete() throw()
{}

template <typename T>
template <typename U>
default_delete<T>::default_delete(
    const default_delete<U>& /*unused*/,
    typename ft::enable_if<ft::is_convertible<U*, T*>::value,
                           _enabler>::type /*unused = _enabler()*/) throw()
{}

template <typename T>
void default_delete<T>::operator()(T* ptr) const
{
	FT_STATIC_ASSERT( // Cannot delete an incomplete type.
	    ft::is_complete<T>::value);

	delete ptr;
}

/* default_delete<T[]> */

template <typename T>
default_delete<T[]>::default_delete() throw()
{}

template <typename T>
template <typename U>
default_delete<T[]>::default_delete(
    const default_delete<U[]>& /*unused*/,
    typename ft::enable_if<ft::is_convertible<U (*)[], T (*)[]>::value,
                           _enabler>::type /*unused = _enabler()*/) throw()
{}

template <typename T>
template <typename U>
FT_REQUIRES((ft::is_convertible<U (*)[], T (*)[]>::value))
(void)default_delete<T[]>::operator()(U* ptr) const
{
	FT_STATIC_ASSERT( // Cannot delete an incomplete type.
	    ft::is_complete<U>::value);

	delete[] ptr;
}

} // namespace ft

#endif // LIBFTPP_MEMORY_DEFAULT_DELETE_TPP
