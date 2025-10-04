// IWYU pragma: private; include "libftpp/functional.hpp"
#pragma once
#ifndef LIBFTPP_FUNCTIONAL_COMPARISONS_TPP
#	define LIBFTPP_FUNCTIONAL_COMPARISONS_TPP

#	include "libftpp/functional.hpp"
#	include <functional>

namespace ft {

/* equal_to */

template <typename T /*= void*/>
bool equal_to<T>::operator()(const T& lhs, const T& rhs) const
{
	return std::equal_to<T>()(lhs, rhs);
}

template <typename T, typename U>
bool equal_to<void>::operator()(const T& lhs, const U& rhs) const
{
	return lhs == rhs;
}

template <class T, class U>
bool equal_to<void>::operator()(T* a, U* b) const
{
	return std::equal_to<const volatile void*>()(a, b);
}

/* not_equal_to */

template <typename T /*= void*/>
bool not_equal_to<T>::operator()(const T& lhs, const T& rhs) const
{
	return std::not_equal_to<T>()(lhs, rhs);
}

template <typename T, typename U>
bool not_equal_to<void>::operator()(const T& lhs, const U& rhs) const
{
	return lhs != rhs;
}

template <class T, class U>
bool not_equal_to<void>::operator()(T* a, U* b) const
{
	return std::not_equal_to<const volatile void*>()(a, b);
}

/* greater */

template <typename T /*= void*/>
bool greater<T>::operator()(const T& lhs, const T& rhs) const
{
	return std::greater<T>()(lhs, rhs);
}

template <typename T, typename U>
bool greater<void>::operator()(const T& lhs, const U& rhs) const
{
	return lhs > rhs;
}

template <class T, class U>
bool greater<void>::operator()(T* a, U* b) const
{
	return std::greater<const volatile void*>()(a, b);
}

/* less */

template <typename T /*= void*/>
bool less<T>::operator()(const T& lhs, const T& rhs) const
{
	return std::less<T>()(lhs, rhs);
}

template <typename T, typename U>
bool less<void>::operator()(const T& lhs, const U& rhs) const
{
	return lhs < rhs;
}

template <class T, class U>
bool less<void>::operator()(T* a, U* b) const
{
	return std::less<const volatile void*>()(a, b);
}

/* greater_equal */

template <typename T /*= void*/>
bool greater_equal<T>::operator()(const T& lhs, const T& rhs) const
{
	return std::greater_equal<T>()(lhs, rhs);
}

template <typename T, typename U>
bool greater_equal<void>::operator()(const T& lhs, const U& rhs) const
{
	return lhs >= rhs;
}

template <class T, class U>
bool greater_equal<void>::operator()(T* a, U* b) const
{
	return std::greater_equal<const volatile void*>()(a, b);
}

/* less_equal */

template <typename T /*= void*/>
bool less_equal<T>::operator()(const T& lhs, const T& rhs) const
{
	return std::less_equal<T>()(lhs, rhs);
}

template <typename T, typename U>
bool less_equal<void>::operator()(const T& lhs, const U& rhs) const
{
	return lhs <= rhs;
}

template <class T, class U>
bool less_equal<void>::operator()(T* a, U* b) const
{
	return std::less_equal<const volatile void*>()(a, b);
}

} // namespace ft

#endif // LIBFTPP_FUNCTIONAL_COMPARISONS_TPP
