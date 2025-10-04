#pragma once

#include "libftpp/functional.hpp"

#define LIBFTPP_REQUIRES_IMPL(EXPRESSION)            \
	typename ft::enable_if < bool(EXPRESSION),       \
	    FT_RETURN_TYPE_MUST_BE_PLACED_IN_PARENTHESES

/* T is put in a function type to remove multiple parentheses. */
#define FT_RETURN_TYPE_MUST_BE_PLACED_IN_PARENTHESES(T)                \
	typename ft::_requires::unpack_return_type<void(T)>::type > ::type

namespace ft {
namespace _requires {

template <typename F>
struct unpack_return_type : ft::function_traits<F>::template argument_type<> {};

} // namespace _requires
} // namespace ft
