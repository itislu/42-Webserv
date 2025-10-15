#pragma once
#ifndef LIBFTPP_MEMORY_MEMORY_UNIQUE_PTR_DETAIL_HPP
#	define LIBFTPP_MEMORY_MEMORY_UNIQUE_PTR_DETAIL_HPP

namespace ft {
namespace _unique_ptr {

/**
 * `remove_reference<Deleter>::type::pointer` if that type exists, otherwise
 * `T*`.
 */
template <typename T, typename Deleter, typename = void>
struct pointer;

template <typename E, typename Deleter>
struct is_compatible_deleter;

} // namespace _unique_ptr
} // namespace ft

#	include "unique_ptr_detail.tpp" // IWYU pragma: export

#endif // LIBFTPP_MEMORY_MEMORY_UNIQUE_PTR_DETAIL_HPP
