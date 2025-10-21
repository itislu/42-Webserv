// IWYU pragma: private; include "shared_ptr_detail.hpp"
#pragma once
#ifndef LIBFTPP_MEMORY_SHARED_PTR_ACCESS_TPP
#	define LIBFTPP_MEMORY_SHARED_PTR_ACCESS_TPP

#	include "shared_ptr_detail.hpp"
#	include "libftpp/type_traits.hpp"
#	include "libftpp/utility.hpp"
#	include <cassert>
#	include <cstddef>

namespace ft {
namespace _shared_ptr {

/* shared_ptr_get */

template <typename T>
typename shared_ptr_get<T>::element_type* shared_ptr_get<T>::get() const throw()
{
	return static_cast<const shared_ptr<T>*>(this)->get();
}

/* shared_ptr_access - non-array, non-void */

template <typename T, typename U /*= void*/>
T& shared_ptr_access<T, U>::operator*() const throw()
{
	assert(this->get() != FT_NULLPTR);
	return *this->get();
}

template <typename T, typename U /*= void*/>
T* shared_ptr_access<T, U>::operator->() const throw()
{
	assert(this->get() != FT_NULLPTR);
	return this->get();
}

/* shared_ptr_access - array */

template <typename T>
typename shared_ptr_get<T>::element_type&
shared_ptr_access<T, typename ft::enable_if<ft::is_array<T>::value>::type>::
operator[](std::ptrdiff_t idx) const
{
	assert(this->get() != FT_NULLPTR && idx >= 0);
	if (ft::is_bounded_array<T>::value) {
		assert(static_cast<std::size_t>(idx) < ft::extent<T>::value);
	}
	return this->get()[idx];
}

/* shared_ptr_access - void */

template <typename T>
T* shared_ptr_access<T, typename ft::enable_if<ft::is_void<T>::value>::type>::
operator->() const throw()
{
	assert(this->get() != FT_NULLPTR);
	return this->get();
}

} // namespace _shared_ptr
} // namespace ft

#endif // LIBFTPP_MEMORY_SHARED_PTR_ACCESS_TPP
