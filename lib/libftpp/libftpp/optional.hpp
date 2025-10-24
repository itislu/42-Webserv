#pragma once
#ifndef LIBFTPP_OPTIONAL_HPP
#	define LIBFTPP_OPTIONAL_HPP

#	include "libftpp/assert.hpp"
#	include "libftpp/safe_bool.hpp"
#	include "libftpp/type_traits.hpp"
#	include <exception>

namespace ft {

struct nullopt_t;
class bad_optional_access;

/**
 * https://en.cppreference.com/w/cpp/utility/optional
 */
template <typename T>
class optional : public ft::safe_bool<optional<T> > {
private:
	FT_STATIC_ASSERT( // T must not be a reference type.
	    !ft::is_reference<T>::value);
	FT_STATIC_ASSERT( // T must not be a function type.
	    !ft::is_function<T>::value);
	FT_STATIC_ASSERT( // T must not be an array type.
	    !ft::is_array<T>::value);
	FT_STATIC_ASSERT( // T must not be (possibly cv-qualified) void.
	    !ft::is_void<T>::value);
	FT_STATIC_ASSERT( // T must not be ft::nullopt_t.
	    (!ft::is_same<typename ft::remove_cv<T>::type, ft::nullopt_t>::value));

public:
	optional() throw();
	// Original in `std::optional` is not `explicit`.
	// NOLINTNEXTLINE(google-explicit-constructor)
	optional(nullopt_t /*unused*/) throw();
	optional(const optional& other);
	// Originals in `std::optional` are conditionally `explicit`.
	// NOLINTBEGIN(google-explicit-constructor)
	template <typename U>
	optional(const optional<U>& other);
	template <typename U>
	optional(const U& value);
	// NOLINTEND(google-explicit-constructor)
	~optional();
	optional& operator=(nullopt_t /*unused*/) throw();
	optional& operator=(optional other) throw();

	const T* operator->() const throw();
	T* operator->() throw();
	const T& operator*() const throw();
	T& operator*() throw();
	bool operator_bool() const throw();
	bool has_value() const throw();
	const T& value() const;
	T& value();
	template <typename U>
	T value_or(const U& default_value) const;

	template <typename F>
	optional and_then(const F& f) const;
	template <typename F>
	optional transform(const F& f) const;
	template <typename F>
	optional or_else(const F& f) const;

	void swap(optional& other) throw();
	void reset() throw();

private:
	T* _value;
};

template <typename T>
optional<T> make_optional(const T& value);
template <typename T>
void swap(optional<T>& lhs, optional<T>& rhs) throw();

template <typename T, typename U>
bool operator==(const optional<T>& lhs, const optional<U>& rhs);
template <typename T, typename U>
bool operator!=(const optional<T>& lhs, const optional<U>& rhs);
template <typename T, typename U>
bool operator<(const optional<T>& lhs, const optional<U>& rhs);
template <typename T, typename U>
bool operator<=(const optional<T>& lhs, const optional<U>& rhs);
template <typename T, typename U>
bool operator>(const optional<T>& lhs, const optional<U>& rhs);
template <typename T, typename U>
bool operator>=(const optional<T>& lhs, const optional<U>& rhs);

template <typename T>
bool operator==(const optional<T>& opt, nullopt_t /*unused*/) throw();
template <typename T>
bool operator==(nullopt_t /*unused*/, const optional<T>& opt) throw();
template <typename T>
bool operator!=(const optional<T>& opt, nullopt_t /*unused*/) throw();
template <typename T>
bool operator!=(nullopt_t /*unused*/, const optional<T>& opt) throw();
template <typename T>
bool operator<(const optional<T>& opt, nullopt_t /*unused*/) throw();
template <typename T>
bool operator<(nullopt_t /*unused*/, const optional<T>& opt) throw();
template <typename T>
bool operator<=(const optional<T>& opt, nullopt_t /*unused*/) throw();
template <typename T>
bool operator<=(nullopt_t /*unused*/, const optional<T>& opt) throw();
template <typename T>
bool operator>(const optional<T>& opt, nullopt_t /*unused*/) throw();
template <typename T>
bool operator>(nullopt_t /*unused*/, const optional<T>& opt) throw();
template <typename T>
bool operator>=(const optional<T>& opt, nullopt_t /*unused*/) throw();
template <typename T>
bool operator>=(nullopt_t /*unused*/, const optional<T>& opt) throw();

template <typename T, typename U>
bool operator==(const optional<T>& opt, const U& value);
template <typename U, typename T>
bool operator==(const U& value, const optional<T>& opt);
template <typename T, typename U>
bool operator!=(const optional<T>& opt, const U& value);
template <typename U, typename T>
bool operator!=(const U& value, const optional<T>& opt);
template <typename T, typename U>
bool operator<(const optional<T>& opt, const U& value);
template <typename U, typename T>
bool operator<(const U& value, const optional<T>& opt);
template <typename T, typename U>
bool operator<=(const optional<T>& opt, const U& value);
template <typename U, typename T>
bool operator<=(const U& value, const optional<T>& opt);
template <typename T, typename U>
bool operator>(const optional<T>& opt, const U& value);
template <typename U, typename T>
bool operator>(const U& value, const optional<T>& opt);
template <typename T, typename U>
bool operator>=(const optional<T>& opt, const U& value);
template <typename U, typename T>
bool operator>=(const U& value, const optional<T>& opt);

/**
 * https://en.cppreference.com/w/cpp/utility/optional/nullopt_t
 */
struct nullopt_t {
	explicit nullopt_t(int /*unused*/) throw();
};

/**
 * https://en.cppreference.com/w/cpp/utility/optional/bad_optional_access
 */
class bad_optional_access : public std::exception {
public:
	const char* what() const throw();
};

/**
 * https://en.cppreference.com/w/cpp/utility/optional/nullopt
 */
extern const nullopt_t nullopt;

} // namespace ft

#	include "libftpp/optional/optional.tpp" // IWYU pragma: export

#endif // LIBFTPP_OPTIONAL_HPP
