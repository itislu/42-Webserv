#pragma once
#ifndef LIBFTPP_TYPE_TRAITS_TYPE_TRAITS_DETAIL_HPP
#	define LIBFTPP_TYPE_TRAITS_TYPE_TRAITS_DETAIL_HPP

namespace ft {
namespace _type_traits {

/**
 * Incomplete type used to represent a NULL type internally in type traits.
 */
struct null_type;

/**
 * Returns `T::type` if that is well-formed, otherwise `null_type`.
 */
template <typename T>
struct type_or_null;

} // namespace _type_traits
} // namespace ft

#	include "type_traits_detail.tpp" // IWYU pragma: export

#endif // LIBFTPP_TYPE_TRAITS_TYPE_TRAITS_DETAIL_HPP
