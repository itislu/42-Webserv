// IWYU pragma: private; include "libftpp/safe_bool.hpp"
#pragma once
#ifndef LIBFTPP_SAFE_BOOL_SAFE_BOOL_TPP
#	define LIBFTPP_SAFE_BOOL_SAFE_BOOL_TPP

#	include "libftpp/safe_bool.hpp"
#	include "safe_bool_base.hpp"
#	include <cstddef>

namespace ft {

template <typename Derived>
safe_bool<Derived>::operator safe_bool_t() const
{
	return (static_cast<const Derived*>(this))->boolean_test()
	           ? &safe_bool_base::this_type_does_not_support_comparisons
	           : NULL;
}

template <typename Derived>
safe_bool<Derived>::safe_bool()
{}

template <typename Derived>
safe_bool<Derived>::~safe_bool()
{}

template <typename T>
bool operator==(const safe_bool<T>& lhs, bool b)
{
	return b == static_cast<bool>(lhs);
}

template <typename T>
bool operator==(bool b, const safe_bool<T>& rhs)
{
	return b == static_cast<bool>(rhs);
}

template <typename T>
bool operator!=(const safe_bool<T>& lhs, bool b)
{
	return b != static_cast<bool>(lhs);
}

template <typename T>
bool operator!=(bool b, const safe_bool<T>& rhs)
{
	return b != static_cast<bool>(rhs);
}

template <typename T, typename U>
bool operator==(const safe_bool<T>& lhs, const U& /*rhs*/)
{
	lhs.this_type_does_not_support_comparisons();
	return false;
}

template <typename T, typename U>
bool operator==(const U& /*lhs*/, const safe_bool<T>& rhs)
{
	rhs.this_type_does_not_support_comparisons();
	return false;
}

template <typename T, typename U>
bool operator!=(const safe_bool<T>& lhs, const U& /*rhs*/)
{
	lhs.this_type_does_not_support_comparisons();
	return false;
}

template <typename T, typename U>
bool operator!=(const U& /*lhs*/, const safe_bool<T>& rhs)
{
	rhs.this_type_does_not_support_comparisons();
	return false;
}

} // namespace ft

#endif // LIBFTPP_SAFE_BOOL_SAFE_BOOL_TPP
