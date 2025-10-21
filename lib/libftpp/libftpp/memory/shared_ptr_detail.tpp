// IWYU pragma: private; include "shared_ptr_detail.hpp"
#pragma once
#ifndef LIBFTPP_MEMORY_SHARED_PTR_DETAIL_TPP
#	define LIBFTPP_MEMORY_SHARED_PTR_DETAIL_TPP

#	include "shared_ptr_detail.hpp"
#	include "libftpp/type_traits.hpp"
#	include <cstddef>

namespace ft {
namespace _shared_ptr {

/* is_compatible_raw_pointee */

template <typename Y, typename T, typename /*= void*/>
struct is_compatible_raw_pointee : ft::false_type {};

template <typename Y, typename U, std::size_t N>
struct is_compatible_raw_pointee<
    Y,
    U[N],
    typename ft::enable_if<ft::is_convertible<Y (*)[N], U (*)[N]>::value>::type>
    : ft::true_type {};

template <typename Y, typename U>
struct is_compatible_raw_pointee<
    Y,
    U[],
    typename ft::enable_if<ft::is_convertible<Y (*)[], U (*)[]>::value>::type>
    : ft::true_type {};

template <typename Y, typename T>
struct is_compatible_raw_pointee<
    Y,
    T,
    typename ft::enable_if<!ft::is_array<T>::value>::type>
    : ft::is_convertible<Y*, T*> {};

/* is_compatible_smart_pointer */

template <typename Y, typename T>
struct _is_compatible_array;

template <typename Y, typename T>
struct is_compatible_smart_pointer
    : ft::bool_constant<ft::is_convertible<Y*, T*>::value
                        || _is_compatible_array<Y, T>::value> {};

template <typename Y, typename T>
struct _is_compatible_array : ft::false_type {};

template <typename U, std::size_t N, typename T>
struct _is_compatible_array<U[N], T>
    : ft::is_same<typename ft::remove_cv<T>::type, U[]> {};

} // namespace _shared_ptr
} // namespace ft

#endif // LIBFTPP_MEMORY_SHARED_PTR_DETAIL_TPP
