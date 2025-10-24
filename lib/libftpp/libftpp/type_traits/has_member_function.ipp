// IWYU pragma: private; include "libftpp/type_traits.hpp"
#pragma once
#ifndef LIBFTPP_TYPE_TRAITS_HAS_MEMBER_FUNCTION_IPP
#	define LIBFTPP_TYPE_TRAITS_HAS_MEMBER_FUNCTION_IPP

// NOLINTBEGIN(bugprone-macro-parentheses)
#	define LIBFTPP_HAS_MEMBER_FUNCTION_IMPL(                                 \
	    RETURN_TYPE, NAME, ARGS_IN_PARENS)                                    \
		namespace detail_has_member_function_##NAME                           \
		{                                                                     \
                                                                              \
			/* 2nd param: member-function pointer non-type template param. */ \
			template <typename T, RETURN_TYPE(T::*) ARGS_IN_PARENS>           \
			struct sfinae {};                                                 \
                                                                              \
			template <typename, typename = void>                              \
			struct impl : ft::false_type {};                                  \
                                                                              \
			template <typename T>                                             \
			struct impl<T, typename ft::voider<sfinae<T, &T::NAME> >::type>   \
			    : ft::true_type {};                                           \
                                                                              \
		} /* namespace detail_has_member_function_##NAME */                   \
                                                                              \
		template <typename T>                                                 \
		struct has_member_function_##NAME                                     \
		    : detail_has_member_function_##NAME::impl<T> {};
// NOLINTEND(bugprone-macro-parentheses)

#endif // LIBFTPP_TYPE_TRAITS_HAS_MEMBER_FUNCTION_IPP
