// IWYU pragma: private; include "libftpp/utility.hpp"
#pragma once
#ifndef LIBFTPP_UTILITY_MOVE_TPP
#	define LIBFTPP_UTILITY_MOVE_TPP

#	include "libftpp/utility.hpp"
#	include "libftpp/movable.hpp"
#	include "libftpp/type_traits.hpp"

namespace ft {

template <typename T>
ft::rvalue<typename ft::remove_reference<T&>::type>& move(T& t) throw()
{
	return static_cast<ft::rvalue<typename ft::remove_reference<T&>::type>&>(t);
}

} // namespace ft

#endif // LIBFTPP_UTILITY_MOVE_TPP
