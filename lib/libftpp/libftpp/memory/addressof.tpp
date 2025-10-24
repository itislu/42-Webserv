// IWYU pragma: private; include "libftpp/memory.hpp"
#pragma once
#ifndef LIBFTPP_MEMORY_ADDRESSOF_TPP
#	define LIBFTPP_MEMORY_ADDRESSOF_TPP

#	include "libftpp/memory.hpp"
#	include "libftpp/type_traits.hpp"

namespace ft {

namespace _addressof {
template <typename T>
FT_REQUIRES(ft::is_object<T>::value)
(T*)addressof(T& arg) throw();
template <typename T>
FT_REQUIRES(!ft::is_object<T>::value)
(T*)addressof(T& arg) throw();
} // namespace _addressof

template <typename T>
T* addressof(T& arg) throw()
{
	return _addressof::addressof(arg);
}

namespace _addressof {

template <typename T>
FT_REQUIRES(ft::is_object<T>::value)
(T*)addressof(T& arg) throw()
{
	// Circumvent operator& overloads.
	// NOLINTBEGIN(cppcoreguidelines-pro-type-*-cast)
	return reinterpret_cast<T*>(
	    &const_cast<char&>(reinterpret_cast<const volatile char&>(arg)));
	// NOLINTEND(cppcoreguidelines-pro-type-*-cast)
}

template <typename T>
FT_REQUIRES(!ft::is_object<T>::value)
(T*)addressof(T& arg) throw() { return &arg; }

} // namespace _addressof

} // namespace ft

#endif // LIBFTPP_MEMORY_ADDRESSOF_TPP
