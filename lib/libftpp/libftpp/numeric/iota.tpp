// IWYU pragma: private; include "libftpp/numeric.hpp"
#pragma once
#ifndef LIBFTPP_NUMERIC_IOTA_TPP
#	define LIBFTPP_NUMERIC_IOTA_TPP

#	include "libftpp/numeric.hpp"

namespace ft {

template <typename ForwardIt, typename T>
void iota(ForwardIt first, ForwardIt last, T value)
{
	for (; first != last; ++first, ++value) {
		*first = value;
	}
}

} // namespace ft

#endif // LIBFTPP_NUMERIC_IOTA_TPP
