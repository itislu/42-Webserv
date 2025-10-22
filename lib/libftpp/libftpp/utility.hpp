#pragma once
#ifndef LIBFTPP_UTILITY_HPP
#	define LIBFTPP_UTILITY_HPP

#	include "libftpp/utility/countof.ipp"
#	include "libftpp/movable.hpp"
#	include "libftpp/type_traits.hpp"
#	include <string>

/**
 * https://en.cppreference.com/w/cpp/utility/unreachable
 */
#	if defined(__GNUC__) || defined(__clang__)
#		define FT_UNREACHABLE() __builtin_unreachable()
#	elif defined(_MSC_VER)
#		define FT_UNREACHABLE() __assume(false)
#	else // defined(__GNUC__) || defined(__clang__)
#		include <exception>
#		define FT_UNREACHABLE() std::terminate()
#	endif // defined(__GNUC__) || defined(__clang__)

/**
 * @brief Macro to get the length of a C-array at compile time in a type-safe
 * manner
 *
 * Inspiration:
 * https://web.archive.org/web/20141223215212/http://www.drdobbs.com/cpp/counting-array-elements-at-compile-time/197800525?pgno=6
 */
#	define FT_COUNTOF(ARRAY) LIBFTPP_COUNTOF_IMPL(ARRAY)

/**
 * @brief `nullptr` emulation
 *
 * Implemented as a macro because `nullptr` is a prvalue.
 * The `static_cast` is necessary to prevent the "most vexing parse".
 *
 * https://en.cppreference.com/w/cpp/language/nullptr
 */
#	define FT_NULLPTR static_cast<ft::nullptr_t>(ft::nullptr_t())

namespace ft {

/* demangle */

std::string demangle(const char* mangled_name);

/* move */

/**
 * @brief Casts an lvalue to an rvalue reference emulation to indicate an object
 * may be moved from
 *
 * Needed for functions taking `ft::rvalue<T>&` parameters to be chosen by
 * overload resolution.
 *
 * `ft::remove_reference<T&>::type` is used to prevent nesting of `ft::rvalue`s.
 *
 * https://en.cppreference.com/w/cpp/utility/move
 */
template <typename T>
ft::rvalue<typename ft::remove_reference<T&>::type>& move(T& t) throw();

/* nullptr_t */

/**
 * @brief `nullptr_t` emulation
 *
 * https://en.cppreference.com/w/cpp/types/nullptr_t
 */
class nullptr_t {
public:
	// NOLINTBEGIN(google-explicit-constructor): `std::nullptr_t` may be
	// implicitly converted to any pointer and pointer to member type.
	template <typename T>
	operator T*() const throw();

	template <typename C, typename T>
	operator T C::*() const throw();
	// NOLINTEND(google-explicit-constructor)

private:
	// `sizeof(std::nullptr_t)` is equal to `sizeof(void *)`.
	void* _;
};

// Disabled ordered comparison operators.
template <typename T>
bool operator<(T* /*unused*/, nullptr_t /*unused*/);
template <typename T>
bool operator<(nullptr_t /*unused*/, T* /*unused*/);
template <typename T>
bool operator<=(T* /*unused*/, nullptr_t /*unused*/);
template <typename T>
bool operator<=(nullptr_t /*unused*/, T* /*unused*/);
template <typename T>
bool operator>(T* /*unused*/, nullptr_t /*unused*/);
template <typename T>
bool operator>(nullptr_t /*unused*/, T* /*unused*/);
template <typename T>
bool operator>=(T* /*unused*/, nullptr_t /*unused*/);
template <typename T>
bool operator>=(nullptr_t /*unused*/, T* /*unused*/);

} // namespace ft

#	include "libftpp/utility/move.tpp"      // IWYU pragma: export
#	include "libftpp/utility/nullptr_t.tpp" // IWYU pragma: export

#endif // LIBFTPP_UTILITY_HPP
