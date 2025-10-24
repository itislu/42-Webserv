// IWYU pragma: private; include "type_traits_detail.hpp"
#pragma once
#ifndef LIBFTPP_TYPE_TRAITS_TYPE_TRAITS_DETAIL_TPP
#	define LIBFTPP_TYPE_TRAITS_TYPE_TRAITS_DETAIL_TPP

#	include "type_traits_detail.hpp"
#	include "libftpp/type_traits.hpp"

namespace ft {
namespace _type_traits {

/* is_rvalue */
template <typename>
struct is_rvalue : false_type {};

template <typename T>
struct is_rvalue<ft::rvalue<T> > : true_type {};

template <typename T>
struct is_rvalue<const ft::rvalue<T> > : true_type {};

template <typename T>
struct is_rvalue<volatile ft::rvalue<T> > : true_type {};

template <typename T>
struct is_rvalue<const volatile ft::rvalue<T> > : true_type {};

/* type_or_null */
namespace _type_or_null {
template <typename T, typename = void>
struct impl;
} // namespace _type_or_null

template <typename T>
struct type_or_null : _type_or_null::impl<T> {};

namespace _type_or_null {

template <typename, typename /*= void*/>
struct impl : type_identity<null_type> {};

template <typename T>
struct impl<T, typename voider<typename T::type>::type>
    : type_identity<typename T::type> {};

} // namespace _type_or_null

} // namespace _type_traits
} // namespace ft

#endif // LIBFTPP_TYPE_TRAITS_TYPE_TRAITS_DETAIL_TPP
