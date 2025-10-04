// IWYU pragma: private; include "libftpp/expected.hpp"
#pragma once
#ifndef LIBFTPP_EXPECTED_UNEXPECTED_TPP
#	define LIBFTPP_EXPECTED_UNEXPECTED_TPP

#	include "libftpp/expected.hpp"
#	include <algorithm>

namespace ft {

template <typename E>
template <typename Err>
unexpected<E>::unexpected(const Err& e)
    : _error(e)
{}

template <typename E>
const E& unexpected<E>::error() const throw()
{
	return _error;
}

template <typename E>
E& unexpected<E>::error() throw()
{
	return _error;
}

template <typename E>
void unexpected<E>::swap(unexpected& other)
{
	using std::swap;
	swap(_error, other._error);
}

template <typename E>
void swap(unexpected<E>& x, unexpected<E>& y)
{
	x.swap(y);
}

template <typename E, typename E2>
bool operator==(const unexpected<E>& x, const unexpected<E2>& y)
{
	return x.error() == y.error();
}

template <typename E, typename E2>
bool operator!=(const unexpected<E>& x, const unexpected<E2>& y)
{
	return !(x == y);
}

namespace _unexpected {

template <typename>
struct is_unexpected {
	enum {
		value = false
	};
};

template <typename T>
struct is_unexpected<unexpected<T> > {
	enum {
		value = true
	};
};

} // namespace _unexpected

} // namespace ft

#endif // LIBFTPP_EXPECTED_UNEXPECTED_TPP
