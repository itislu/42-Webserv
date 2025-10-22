#pragma once
#ifndef LIBFTPP_TYPE_TRAITS_TYPE_TRAITS_DETAIL_HPP
#	define LIBFTPP_TYPE_TRAITS_TYPE_TRAITS_DETAIL_HPP

namespace ft {
namespace _type_traits {

/**
 * @brief Returns `true` if `T` is an `ft::rvalue`
 */
template <typename T>
struct is_rvalue;

/**
 * @brief Incomplete type used to represent a NULL type internally in type
 * traits
 */
struct null_type;

/**
 * @brief Returns `T::type` if that is well-formed, otherwise `null_type`
 */
template <typename T>
struct type_or_null;

} // namespace _type_traits
} // namespace ft

#	include "type_traits_detail.tpp" // IWYU pragma: export

#endif // LIBFTPP_TYPE_TRAITS_TYPE_TRAITS_DETAIL_HPP
