#pragma once
#ifndef LIBFTPP_MOVABLE_MOVABLE_DETAIL_HPP
#	define LIBFTPP_MOVABLE_MOVABLE_DETAIL_HPP

namespace ft {
namespace _movable {

template <typename T, typename = void>
struct inherit_if_class_or_union;

} // namespace _movable
} // namespace ft

#	include "movable_detail.tpp" // IWYU pragma: export

#endif // LIBFTPP_MOVABLE_MOVABLE_DETAIL_HPP
