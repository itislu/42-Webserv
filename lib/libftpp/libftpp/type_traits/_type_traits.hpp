#pragma once

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

#include "_type_traits.tpp" // IWYU pragma: export
