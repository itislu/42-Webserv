// IWYU pragma: private; include "libftpp/optional.hpp"
#pragma once
#ifndef LIBFTPP_OPTIONAL_OPTIONAL_TPP
#	define LIBFTPP_OPTIONAL_OPTIONAL_TPP

#	include "libftpp/optional.hpp"
#	include "libftpp/safe_bool.hpp"
#	include "libftpp/utility.hpp"
#	include <algorithm>
#	include <cassert>

namespace ft {

template <typename T>
optional<T>::optional() throw()
    : _value(FT_NULLPTR)
{}

template <typename T>
optional<T>::optional(nullopt_t /*unused*/) throw()
    : _value(FT_NULLPTR)
{}

template <typename T>
optional<T>::optional(const optional& other)
    : ft::safe_bool<optional>(),
      _value(other.has_value() ? new T(*other._value) : FT_NULLPTR)
{}

template <typename T>
template <typename U>
optional<T>::optional(const optional<U>& other)
    : _value(other.has_value() ? new T(*other) : FT_NULLPTR)
{}

template <typename T>
template <typename U>
optional<T>::optional(const U& value)
    : _value(new T(value))
{}

template <typename T>
optional<T>::~optional()
{
	delete _value;
}

template <typename T>
optional<T>& optional<T>::operator=(nullopt_t /*unused*/) throw()
{
	reset();
	return *this;
}

template <typename T>
optional<T>& optional<T>::operator=(optional other) throw()
{
	swap(other);
	return *this;
}

template <typename T>
const T* optional<T>::operator->() const throw()
{
	assert(has_value());
	return _value;
}

template <typename T>
T* optional<T>::operator->() throw()
{
	assert(has_value());
	return _value;
}

template <typename T>
const T& optional<T>::operator*() const throw()
{
	assert(has_value());
	return *_value;
}

template <typename T>
T& optional<T>::operator*() throw()
{
	assert(has_value());
	return *_value;
}

template <typename T>
bool optional<T>::boolean_test() const throw()
{
	return has_value();
}

template <typename T>
bool optional<T>::has_value() const throw()
{
	return _value != FT_NULLPTR;
}

template <typename T>
const T& optional<T>::value() const
{
	if (has_value()) {
		return *_value;
	}
	throw bad_optional_access();
}

template <typename T>
T& optional<T>::value()
{
	if (has_value()) {
		return *_value;
	}
	throw bad_optional_access();
}

template <typename T>
template <typename U>
T optional<T>::value_or(const U& default_value) const
{
	if (has_value()) {
		return *_value;
	}
	return static_cast<T>(default_value);
}

/**
 * The monadic functions are limited in their use compared to the original ones.
 * The functions passed in the original ones can return different result or
 * error types, but in this implementation the limits of C++98 and the lack of
 * the `auto` keyword and `result_of` and `decltype` metaprogramming features
 * make it very difficult to come close to that.
 */

/**
 * f should return an optional<T>
 */
template <typename T>
template <typename F>
optional<T> optional<T>::and_then(const F& f) const
{
	if (has_value()) {
		return f(*_value);
	}
	return optional();
}

/**
 * f should return a value of type T
 */
template <typename T>
template <typename F>
optional<T> optional<T>::transform(const F& f) const
{
	if (has_value()) {
		return optional(f(*_value));
	}
	return optional();
}

/**
 * f should return an optional<T>
 */
template <typename T>
template <typename F>
optional<T> optional<T>::or_else(const F& f) const
{
	if (!has_value()) {
		return f();
	}
	return *this;
}

template <typename T>
void optional<T>::swap(optional& other) throw()
{
	using std::swap;
	swap(_value, other._value);
}

template <typename T>
void optional<T>::reset() throw()
{
	if (has_value()) {
		delete _value;
		_value = FT_NULLPTR;
	}
}

template <typename T>
optional<T> make_optional(const T& value)
{
	return optional<T>(value);
}

template <typename T>
void swap(optional<T>& lhs, optional<T>& rhs) throw()
{
	lhs.swap(rhs);
}

template <typename T, typename U>
bool operator==(const optional<T>& lhs, const optional<U>& rhs)
{
	if (lhs.has_value() != rhs.has_value()) {
		return false;
	}
	if (!lhs.has_value()) {
		return true;
	}
	return *lhs == *rhs;
}

template <typename T, typename U>
bool operator!=(const optional<T>& lhs, const optional<U>& rhs)
{
	return !(lhs == rhs);
}

template <typename T, typename U>
bool operator<(const optional<T>& lhs, const optional<U>& rhs)
{
	if (!rhs.has_value()) {
		return false;
	}
	if (!lhs.has_value()) {
		return true;
	}
	return *lhs < *rhs;
}

template <typename T, typename U>
bool operator<=(const optional<T>& lhs, const optional<U>& rhs)
{
	return !(lhs > rhs);
}

template <typename T, typename U>
bool operator>(const optional<T>& lhs, const optional<U>& rhs)
{
	return rhs < lhs;
}

template <typename T, typename U>
bool operator>=(const optional<T>& lhs, const optional<U>& rhs)
{
	return !(lhs < rhs);
}

template <typename T>
bool operator==(const optional<T>& opt, nullopt_t /*unused*/) throw()
{
	return !opt.has_value();
}

template <typename T>
bool operator==(nullopt_t /*unused*/, const optional<T>& opt) throw()
{
	return opt == nullopt;
}

template <typename T>
bool operator!=(const optional<T>& opt, nullopt_t /*unused*/) throw()
{
	return !(opt == nullopt);
}

template <typename T>
bool operator!=(nullopt_t /*unused*/, const optional<T>& opt) throw()
{
	return !(opt == nullopt);
}

template <typename T>
bool operator<(const optional<T>& /*opt*/, nullopt_t /*unused*/) throw()
{
	return false;
}

template <typename T>
bool operator<(nullopt_t /*unused*/, const optional<T>& opt) throw()
{
	return opt.has_value();
}

template <typename T>
bool operator<=(const optional<T>& opt, nullopt_t /*unused*/) throw()
{
	return !(opt > nullopt);
}

template <typename T>
bool operator<=(nullopt_t /*unused*/, const optional<T>& opt) throw()
{
	return !(nullopt > opt);
}

template <typename T>
bool operator>(const optional<T>& opt, nullopt_t /*unused*/) throw()
{
	return nullopt < opt;
}

template <typename T>
bool operator>(nullopt_t /*unused*/, const optional<T>& opt) throw()
{
	return opt < nullopt;
}

template <typename T>
bool operator>=(const optional<T>& opt, nullopt_t /*unused*/) throw()
{
	return !(opt < nullopt);
}

template <typename T>
bool operator>=(nullopt_t /*unused*/, const optional<T>& opt) throw()
{
	return !(nullopt < opt);
}

template <typename T, typename U>
bool operator==(const optional<T>& opt, const U& value)
{
	return opt.has_value() && *opt == value;
}

template <typename U, typename T>
bool operator==(const U& value, const optional<T>& opt)
{
	return opt == value;
}

template <typename T, typename U>
bool operator!=(const optional<T>& opt, const U& value)
{
	return !(opt == value);
}

template <typename U, typename T>
bool operator!=(const U& value, const optional<T>& opt)
{
	return !(opt == value);
}

template <typename T, typename U>
bool operator<(const optional<T>& opt, const U& value)
{
	return !opt.has_value() || *opt < value;
}

template <typename U, typename T>
bool operator<(const U& value, const optional<T>& opt)
{
	return opt.has_value() && value < *opt;
}

template <typename T, typename U>
bool operator<=(const optional<T>& opt, const U& value)
{
	return !(opt > value);
}

template <typename U, typename T>
bool operator<=(const U& value, const optional<T>& opt)
{
	return !(value > opt);
}

template <typename T, typename U>
bool operator>(const optional<T>& opt, const U& value)
{
	return value < opt;
}

template <typename U, typename T>
bool operator>(const U& value, const optional<T>& opt)
{
	return opt < value;
}

template <typename T, typename U>
bool operator>=(const optional<T>& opt, const U& value)
{
	return !(opt < value);
}

template <typename U, typename T>
bool operator>=(const U& value, const optional<T>& opt)
{
	return !(value < opt);
}

} // namespace ft

#endif // LIBFTPP_OPTIONAL_OPTIONAL_TPP
