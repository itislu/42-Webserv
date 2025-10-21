#pragma once
#ifndef LIBFTPP_MEMORY_SHARED_PTR_DETAIL_HPP
#	define LIBFTPP_MEMORY_SHARED_PTR_DETAIL_HPP

namespace ft {
namespace _shared_ptr {

/**
 * If T is an array type U[N], checks whether Y(*)[N] is a valid type and
 * convertible to T*. If T is an array type U[], checks whether Y(*)[] is a
 * valid type convertible to T*. Otherwise, checks whether Y* is convertible to
 * T*.
 */
template <typename Y, typename T, typename = void>
struct is_compatible_raw_pointee;

/**
 * A pointer type Y* is said to be compatible with a pointer type T* if either
 * Y* is convertible to T* or Y is the array type U[N] and T is U cv [] (where
 * cv is some set of cv-qualifiers).
 */
template <typename Y, typename T>
struct is_compatible_smart_pointer;

} // namespace _shared_ptr
} // namespace ft

#	include "shared_ptr_detail.tpp" // IWYU pragma: export

#endif // LIBFTPP_MEMORY_SHARED_PTR_DETAIL_HPP
