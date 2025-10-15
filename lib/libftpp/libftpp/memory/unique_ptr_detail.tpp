#pragma once
#ifndef LIBFTPP_MEMORY_UNIQUE_PTR_DETAIL_TPP
#	define LIBFTPP_MEMORY_UNIQUE_PTR_DETAIL_TPP

#	include "unique_ptr_detail.hpp"
#	include "libftpp/type_traits.hpp"

namespace ft {
namespace _unique_ptr {

/* pointer */

template <typename T, typename, typename /*= void*/>
struct pointer : ft::type_identity<T*> {};

template <typename T, typename Deleter>
struct pointer<T,
               Deleter,
               typename ft::voider<
                   typename ft::remove_reference<Deleter>::type::pointer>::type>
    : ft::type_identity<typename ft::remove_reference<Deleter>::type::pointer> {
};

/* is_compatible_deleter */

template <typename E, typename Deleter>
struct is_compatible_deleter
    : ft::bool_constant<ft::is_reference<Deleter>::value
                            ? ft::is_same<E, Deleter>::value
                            : ft::is_convertible<E, Deleter>::value> {};

} // namespace _unique_ptr
} // namespace ft

#endif // LIBFTPP_MEMORY_UNIQUE_PTR_DETAIL_TPP
