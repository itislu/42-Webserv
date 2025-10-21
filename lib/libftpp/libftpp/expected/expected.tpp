// IWYU pragma: private; include "libftpp/expected.hpp"
#pragma once
#ifndef LIBFTPP_EXPECTED_EXPECTED_TPP
#	define LIBFTPP_EXPECTED_EXPECTED_TPP

#	include "libftpp/expected.hpp"
#	include "libftpp/safe_bool.hpp"
#	include <algorithm>
#	include <cassert>

namespace ft {

// NOLINTBEGIN(cppcoreguidelines-pro-type-union-access)

template <typename T, typename E>
expected<T, E>::expected()
    : _value(new T()),
      _has_value(true)
{}

template <typename T, typename E>
expected<T, E>::expected(const expected& other)
    : ft::safe_bool<expected>(),
      _has_value(false)
{
	if (other._has_value) {
		_value = new T(*other._value);
		_has_value = true;
	}
	else {
		_error = new E(*other._error);
	}
}

template <typename T, typename E>
template <typename U, typename G>
expected<T, E>::expected(const expected<U, G>& other)
    : _has_value(false)
{
	if (other.has_value()) {
		_value = new T(*other);
		_has_value = true;
	}
	else {
		_error = new E(other.error());
	}
}

template <typename T, typename E>
template <typename U>
expected<T, E>::expected(const U& v)
    : _value(new T(v)),
      _has_value(true)
{}

template <typename T, typename E>
template <typename G>
expected<T, E>::expected(const unexpected<G>& e)
    : _error(new E(e.error())),
      _has_value(false)
{}

template <typename T, typename E>
expected<T, E>::expected(unexpect_t /*unused*/)
    : _error(new E()),
      _has_value(false)
{}

template <typename T, typename E>
expected<T, E>::~expected()
{
	if (_has_value) {
		delete _value;
	}
	else {
		delete _error;
	}
}

template <typename T, typename E>
expected<T, E>& expected<T, E>::operator=(expected other) throw()
{
	swap(other);
	return *this;
}

template <typename T, typename E>
const T* expected<T, E>::operator->() const throw()
{
	assert(_has_value);
	return _value;
}

template <typename T, typename E>
T* expected<T, E>::operator->() throw()
{
	assert(_has_value);
	return _value;
}

template <typename T, typename E>
const T& expected<T, E>::operator*() const throw()
{
	assert(_has_value);
	return *_value;
}

template <typename T, typename E>
T& expected<T, E>::operator*() throw()
{
	assert(_has_value);
	return *_value;
}

template <typename T, typename E>
bool expected<T, E>::boolean_test() const throw()
{
	return _has_value;
}

template <typename T, typename E>
bool expected<T, E>::has_value() const throw()
{
	return _has_value;
}

template <typename T, typename E>
const T& expected<T, E>::value() const
{
	if (!_has_value) {
		throw bad_expected_access<E>(*_error);
	}
	return *_value;
}

template <typename T, typename E>
T& expected<T, E>::value()
{
	if (!_has_value) {
		throw bad_expected_access<E>(*_error);
	}
	return *_value;
}

template <typename T, typename E>
const E& expected<T, E>::error() const throw()
{
	assert(!_has_value);
	return *_error;
}

template <typename T, typename E>
E& expected<T, E>::error() throw()
{
	assert(!_has_value);
	return *_error;
}

template <typename T, typename E>
template <typename U>
T expected<T, E>::value_or(const U& default_value) const
{
	if (_has_value) {
		return *_value;
	}
	return static_cast<T>(default_value);
}

template <typename T, typename E>
template <typename G>
E expected<T, E>::error_or(const G& default_value) const
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
 * f should return an expected<T, E>
 */
template <typename T, typename E>
template <typename F>
expected<T, E> expected<T, E>::and_then(const F& f) const
{
	if (_has_value) {
		return f(*_value);
	}
	return *this;
}

/**
 * f should return a value of type T
 */
template <typename T, typename E>
template <typename F>
expected<T, E> expected<T, E>::transform(const F& f) const
{
	if (_has_value) {
		return expected(f(*_value));
	}
	return *this;
}

/**
 * f should return an expected<T, E>
 */
template <typename T, typename E>
template <typename F>
expected<T, E> expected<T, E>::or_else(const F& f) const
{
	if (!_has_value) {
		return f(*_error);
	}
	return *this;
}

/**
 * f should return an unexpected value of type E
 */
template <typename T, typename E>
template <typename F>
expected<T, E> expected<T, E>::transform_error(const F& f) const
{
	if (!_has_value) {
		return expected(unexpected<E>(f(*_error)));
	}
	return *this;
}

template <typename T, typename E>
void expected<T, E>::swap(expected& other) throw()
{
	using std::swap;
	if (_has_value && other._has_value) {
		swap(_value, other._value);
	}
	else if (_has_value) {
		T* tmp = _value;
		_error = other._error;
		_has_value = false;
		other._value = tmp;
		other._has_value = true;
	}
	else if (other._has_value) {
		T* tmp = other._value;
		other._error = _error;
		other._has_value = false;
		_value = tmp;
		_has_value = true;
	}
	else {
		swap(_error, other._error);
	}
}

template <typename T, typename E>
void swap(expected<T, E>& lhs, expected<T, E>& rhs) throw()
{
	lhs.swap(rhs);
}

template <typename T, typename E, typename T2, typename E2>
bool operator==(const expected<T, E>& lhs, const expected<T2, E2>& rhs)
{
	if (lhs.has_value() != rhs.has_value()) {
		return false;
	}
	if (!lhs.has_value()) {
		return lhs.error() == rhs.error();
	}
	return *lhs == *rhs;
}

template <typename T, typename E, typename E2>
bool operator==(const expected<T, E>& lhs, const unexpected<E2>& unex)
{
	return !lhs.has_value() && lhs.error() == unex.error();
}

template <typename T, typename E, typename T2>
bool operator==(const expected<T, E>& lhs, const T2& val)
{
	return lhs.has_value() && *lhs == val;
}

template <typename T, typename E, typename T2, typename E2>
bool operator!=(const expected<T, E>& lhs, const expected<T2, E2>& rhs)
{
	return !(lhs == rhs);
}

template <typename T, typename E, typename E2>
bool operator!=(const expected<T, E>& lhs, const unexpected<E2>& unex)
{
	return !(lhs == unex);
}

template <typename T, typename E, typename T2>
bool operator!=(const expected<T, E>& lhs, const T2& val)
{
	return !(lhs == val);
}

// NOLINTEND(cppcoreguidelines-pro-type-union-access)

} // namespace ft

#endif // LIBFTPP_EXPECTED_EXPECTED_TPP
