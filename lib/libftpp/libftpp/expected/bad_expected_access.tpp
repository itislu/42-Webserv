// IWYU pragma: private; include "libftpp/expected.hpp"
#pragma once

#include "libftpp/expected.hpp"

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
