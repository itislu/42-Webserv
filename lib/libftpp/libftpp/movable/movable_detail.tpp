// IWYU pragma: private; include "movable_detail.hpp"
#pragma once
#ifndef LIBFTPP_MOVABLE_MOVABLE_DETAIL_TPP
#	define LIBFTPP_MOVABLE_MOVABLE_DETAIL_TPP

#	include "movable_detail.hpp"
#	include "libftpp/type_traits.hpp"

namespace ft {
namespace _movable {

template <typename, typename /*= void*/>
struct inherit_if_class_or_union {};

template <typename T>
struct inherit_if_class_or_union<
    T,
    typename ft::enable_if<ft::is_class_or_union<T>::value>::type> : public T {
};

} // namespace _movable
} // namespace ft

#endif // LIBFTPP_MOVABLE_MOVABLE_DETAIL_TPP
