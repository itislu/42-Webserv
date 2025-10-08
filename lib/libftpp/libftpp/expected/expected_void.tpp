// IWYU pragma: private; include "libftpp/expected.hpp"
#pragma once
#ifndef LIBFTPP_EXPECTED_EXPECTED_VOID_TPP
#	define LIBFTPP_EXPECTED_EXPECTED_VOID_TPP

#	include "libftpp/expected.hpp"
#	include "libftpp/safe_bool.hpp"
#	include <algorithm>
#	include <cassert>
#	include <cstddef>

namespace ft {

template <typename E>
expected<void, E>::expected() throw()
    : _error(NULL),
      _has_value(true)
{}

template <typename E>
expected<void, E>::expected(const expected& other)
    : ft::safe_bool<expected<void, E> >(),
      _error(other._has_value ? NULL : new E(*other._error)),
      _has_value(other._has_value)
{}

template <typename E>
template <typename U, typename G>
expected<void, E>::expected(const expected<U, G>& other)
    : _error(other.has_value() ? NULL : new E(other.error())),
      _has_value(other._has_value())
{}

template <typename E>
template <typename G>
expected<void, E>::expected(const unexpected<G>& e)
    : _error(new E(e.error())),
      _has_value(false)
{}

template <typename E>
expected<void, E>::expected(unexpect_t /*unused*/)
    : _error(new E()),
      _has_value(false)
{}

template <typename E>
expected<void, E>::~expected()
{
	delete _error;
}

template <typename E>
expected<void, E>& expected<void, E>::operator=(expected other) throw()
{
	swap(other);
	return *this;
}

template <typename E>
void expected<void, E>::operator*() const throw()
{
	assert(_has_value);
}

template <typename E>
bool expected<void, E>::boolean_test() const throw()
{
	return _has_value;
}

template <typename E>
bool expected<void, E>::has_value() const throw()
{
	return _has_value;
}

template <typename E>
void expected<void, E>::value() const
{
	if (!_has_value) {
		throw bad_expected_access<E>(*_error);
	}
}

template <typename E>
const E& expected<void, E>::error() const throw()
{
	assert(!_has_value);
	return *_error;
}

template <typename E>
E& expected<void, E>::error() throw()
{
	assert(!_has_value);
	return *_error;
}

template <typename E>
template <typename G>
E expected<void, E>::error_or(const G& default_value) const
{
	if (!_has_value) {
		return *_error;
	}
	return default_value;
}

/**
 * The monadic functions are limited in their use compared to the original ones.
 * The functions passed in the original ones can return different result or
 * error types, but in this implementation the limits of C++98 and the lack of
 * the `auto` keyword and `result_of` and `decltype` metaprogramming features
 * make it very difficult to come close to that.
 */

/**
 * f should return an expected<void, E>
 */
template <typename E>
template <typename F>
expected<void, E> expected<void, E>::and_then(const F& f) const
{
	if (_has_value) {
		return f();
	}
	return *this;
}

/**
 * f should return nothing
 */
template <typename E>
template <typename F>
expected<void, E> expected<void, E>::transform(const F& f) const
{
	if (_has_value) {
		f();
		return expected();
	}
	return *this;
}

/**
 * f should return an expected<void, E>
 */
template <typename E>
template <typename F>
expected<void, E> expected<void, E>::or_else(const F& f) const
{
	if (!_has_value) {
		return f(*_error);
	}
	return *this;
}

/**
 * f should return an unexpected value of type E
 */
template <typename E>
template <typename F>
expected<void, E> expected<void, E>::transform_error(const F& f) const
{
	if (!_has_value) {
		return expected(unexpected<E>(f(*_error)));
	}
	return *this;
}

template <typename E>
void expected<void, E>::swap(expected& other) throw()
{
	using std::swap;
	swap(_error, other._error);
	swap(_has_value, other._has_value);
}

template <typename E, typename T2, typename E2>
bool operator==(const expected<void, E>& lhs, const expected<T2, E2>& rhs)
{
	if (lhs.has_value() != rhs.has_value()) {
		return false;
	}
	if (!lhs.has_value()) {
		return lhs.error() == rhs.error();
	}
	return true;
}

} // namespace ft

#endif // LIBFTPP_EXPECTED_EXPECTED_VOID_TPP
