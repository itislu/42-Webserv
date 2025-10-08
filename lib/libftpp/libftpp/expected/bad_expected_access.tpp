// IWYU pragma: private; include "libftpp/expected.hpp"
#pragma once
#ifndef LIBFTPP_EXPECTED_BAD_EXPECTED_ACCESS_TPP
#	define LIBFTPP_EXPECTED_BAD_EXPECTED_ACCESS_TPP

#	include "libftpp/expected.hpp"

namespace ft {

template <typename E>
bad_expected_access<E>::bad_expected_access(const E& e)
    : _error(e)
{}

template <typename E>
const E& bad_expected_access<E>::error() const throw()
{
	return _error;
}

template <typename E>
E& bad_expected_access<E>::error() throw()
{
	return _error;
}

} // namespace ft

#endif // LIBFTPP_EXPECTED_BAD_EXPECTED_ACCESS_TPP
