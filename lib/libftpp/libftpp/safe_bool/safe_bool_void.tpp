// IWYU pragma: private; include "libftpp/safe_bool.hpp"
#pragma once
#ifndef LIBFTPP_SAFE_BOOL_SAFE_BOOL_VOID_TPP
#	define LIBFTPP_SAFE_BOOL_SAFE_BOOL_VOID_TPP

#	include "libftpp/safe_bool.hpp"
#	include "libftpp/utility.hpp"
#	include "safe_bool_base.hpp"

namespace ft {

inline safe_bool<void>::operator safe_bool_t() const
{
	return operator_bool()
	           ? &safe_bool_base::this_type_does_not_support_comparisons
	           : FT_NULLPTR;
}

inline safe_bool<void>::safe_bool() {}

inline safe_bool<void>::~safe_bool() {}

} // namespace ft

#endif // LIBFTPP_SAFE_BOOL_SAFE_BOOL_VOID_TPP
