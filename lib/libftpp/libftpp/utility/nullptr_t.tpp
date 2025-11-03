// IWYU pragma: private; include "libftpp/utility.hpp"
#pragma once
#ifndef LIBFTPP_UTILITY_NULLPTR_T_TPP
#	define LIBFTPP_UTILITY_NULLPTR_T_TPP

#	include "libftpp/utility.hpp"
#	include "libftpp/assert.hpp"
#	include <cstddef>

namespace ft {

template <typename T>
nullptr_t::operator T*() const throw()
{
	(void)_; // Avoid unused private member warning.
	return NULL;
}

template <typename C, typename T>
nullptr_t::operator T C::*() const throw()
{
	(void)_; // Avoid unused private member warning.
	return NULL;
}

template <typename T>
bool operator<(T* /*unused*/, nullptr_t /*unused*/)
{
	FT_STATIC_ASSERT( // Ordered comparison with ft::nullptr_t not allowed.
	    ft::make_false<T>::value);
	FT_UNREACHABLE();
}

template <typename T>
bool operator<(nullptr_t /*unused*/, T* /*unused*/)
{
	FT_STATIC_ASSERT( // Ordered comparison with ft::nullptr_t not allowed.
	    ft::make_false<T>::value);
	FT_UNREACHABLE();
}

template <typename T>
bool operator<=(T* /*unused*/, nullptr_t /*unused*/)
{
	FT_STATIC_ASSERT( // Ordered comparison with ft::nullptr_t not allowed.
	    ft::make_false<T>::value);
	FT_UNREACHABLE();
}

template <typename T>
bool operator<=(nullptr_t /*unused*/, T* /*unused*/)
{
	FT_STATIC_ASSERT( // Ordered comparison with ft::nullptr_t not allowed.
	    ft::make_false<T>::value);
	FT_UNREACHABLE();
}

template <typename T>
bool operator>(T* /*unused*/, nullptr_t /*unused*/)
{
	FT_STATIC_ASSERT( // Ordered comparison with ft::nullptr_t not allowed.
	    ft::make_false<T>::value);
	FT_UNREACHABLE();
}

template <typename T>
bool operator>(nullptr_t /*unused*/, T* /*unused*/)
{
	FT_STATIC_ASSERT( // Ordered comparison with ft::nullptr_t not allowed.
	    ft::make_false<T>::value);
	FT_UNREACHABLE();
}

template <typename T>
bool operator>=(T* /*unused*/, nullptr_t /*unused*/)
{
	FT_STATIC_ASSERT( // Ordered comparison with ft::nullptr_t not allowed.
	    ft::make_false<T>::value);
	FT_UNREACHABLE();
}

template <typename T>
bool operator>=(nullptr_t /*unused*/, T* /*unused*/)
{
	FT_STATIC_ASSERT( // Ordered comparison with ft::nullptr_t not allowed.
	    ft::make_false<T>::value);
	FT_UNREACHABLE();
}

} // namespace ft

#endif // LIBFTPP_UTILITY_NULLPTR_T_TPP
