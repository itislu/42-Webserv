// IWYU pragma: private; include "libftpp/utility.hpp"
#pragma once
#ifndef LIBFTPP_UTILITY_MOVE_TPP
#	define LIBFTPP_UTILITY_MOVE_TPP

#	include "libftpp/utility.hpp"
#	include "libftpp/movable.hpp"

namespace ft {

template <typename T>
ft::rvalue<T>& move(T& t) throw()
{
	return static_cast<ft::rvalue<T>&>(t);
}

} // namespace ft

#endif // LIBFTPP_UTILITY_MOVE_TPP
