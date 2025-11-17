// IWYU pragma: private; include "libftpp/type_traits.hpp"
#pragma once
#ifndef LIBFTPP_TYPE_TRAITS_TYPE_TRAITS_TPP
#	define LIBFTPP_TYPE_TRAITS_TYPE_TRAITS_TPP

#	include "libftpp/type_traits.hpp"
#	include "type_traits_detail.hpp"
#	include "libftpp/assert.hpp" // IWYU pragma: keep: Conditionally needed.
#	include <cstddef>
#	include <limits>

namespace ft {

/* Forward declarations */

template <typename B1 = _type_traits::null_type,
          typename B2 = _type_traits::null_type,
          typename B3 = _type_traits::null_type,
          typename B4 = _type_traits::null_type,
          typename B5 = _type_traits::null_type,
          typename B6 = _type_traits::null_type,
          typename B7 = _type_traits::null_type,
          typename B8 = _type_traits::null_type,
          typename B9 = _type_traits::null_type,
          typename B10 = _type_traits::null_type>
struct conjunction;
template <typename B1 = _type_traits::null_type,
          typename B2 = _type_traits::null_type,
          typename B3 = _type_traits::null_type,
          typename B4 = _type_traits::null_type,
          typename B5 = _type_traits::null_type,
          typename B6 = _type_traits::null_type,
          typename B7 = _type_traits::null_type,
          typename B8 = _type_traits::null_type,
          typename B9 = _type_traits::null_type,
          typename B10 = _type_traits::null_type>
struct disjunction;

/* Base classes */

/* integral_constant */
template <typename T, T v>
integral_constant<T, v>::operator integral_constant<T, v>::value_type() const
    throw()
{
	return value;
}

template <typename T, T v>
typename integral_constant<T, v>::value_type
integral_constant<T, v>::operator()() const throw()
{
	return value;
}

/**
 * Out-of-class definition required for ODR-usage (taking address or binding to
 * reference).
 * https://listarchives.boost.org/Archives/boost/2003/01/41847.php
 */
template <typename T, T v>
const T integral_constant<T, v>::value;

/* Unary type traits */

/* is_void */
template <typename T>
struct is_void : is_same<typename remove_cv<T>::type, void> {};

/* is_integral */
namespace _is_integral {
template <typename T>
struct impl;
} // namespace _is_integral

/**
 * Short-circuit types which `std::numeric_limits` does not support.
 */
template <typename T>
struct is_integral : conjunction<is_returnable<T>, _is_integral::impl<T> > {};

namespace _is_integral {

template <typename T>
struct impl : bool_constant<std::numeric_limits<T>::is_integer> {};

} // namespace _is_integral

/* is_floating_point */
namespace _is_floating_point {
template <typename T>
struct impl;
} // namespace _is_floating_point

/**
 * Short-circuit types which `std::numeric_limits` does not support.
 */
template <typename T>
struct is_floating_point
    : conjunction<is_returnable<T>, _is_floating_point::impl<T> > {};

namespace _is_floating_point {

template <typename T>
struct impl : bool_constant<std::numeric_limits<T>::is_specialized
                            && !std::numeric_limits<T>::is_integer> {};

} // namespace _is_floating_point

/* is_array */
template <typename>
struct is_array : false_type {};

template <typename T>
struct is_array<T[]> : true_type {};

template <typename T, std::size_t N>
struct is_array<T[N]> : true_type {};

/* is_function */
template <typename T>
struct is_function
    : bool_constant<!is_const<const T>::value && !is_reference<T>::value> {};

/* is_pointer */
template <typename T>
struct is_pointer : conditional<is_const<T>::value || is_volatile<T>::value,
                                is_pointer<typename remove_cv<T>::type>,
                                false_type>::type {};

template <typename T>
struct is_pointer<T*> : true_type {};

/* is_lvalue_reference */
template <typename>
struct is_lvalue_reference : false_type {};

template <typename T>
struct is_lvalue_reference<T&>
    : bool_constant<!is_rvalue_reference<T&>::value> {};

/* is_rvalue_reference */
template <typename>
struct is_rvalue_reference : false_type {};

template <typename T>
struct is_rvalue_reference<ft::rvalue<T>&> : true_type {};

template <typename T>
struct is_rvalue_reference<const ft::rvalue<T>&> : true_type {};

template <typename T>
struct is_rvalue_reference<volatile ft::rvalue<T>&> : true_type {};

template <typename T>
struct is_rvalue_reference<const volatile ft::rvalue<T>&> : true_type {};

/* is_arithmetic */
template <typename T>
struct is_arithmetic
    : bool_constant<is_integral<T>::value || is_floating_point<T>::value> {};

/* is_object */
template <typename T>
struct is_object
    : bool_constant<!is_function<T>::value && !is_reference<T>::value
                    && !is_void<T>::value> {};

/* is_reference */
template <typename T>
struct is_reference : bool_constant<is_lvalue_reference<T>::value
                                    || is_rvalue_reference<T>::value> {};

/* is_const */
template <typename>
struct is_const : false_type {};

template <typename T>
struct is_const<const T> : true_type {};

/* is_volatile */
template <typename>
struct is_volatile : false_type {};

template <typename T>
struct is_volatile<volatile T> : true_type {};

/* is_abstract */
#	if defined(__clang__) && defined(__has_feature)
#		if __has_feature(is_abstract)
#			define LIBFTPP_BUILTIN_IS_ABSTRACT(T) __is_abstract(T)
#		endif // __has_feature(is_abstract)
#	elif defined(__GNUC__)                                         \
	    && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3))
// Introduced to gcc in commit cb68ec50055e516ac270a043f772935561b01968
#		define LIBFTPP_BUILTIN_IS_ABSTRACT(T) __is_abstract(T)
#	elif defined(_MSC_VER) && _MSC_VER >= 1400
// Appears in Visual Studio 2005 Docs, but not in 2003
#		define LIBFTPP_BUILTIN_IS_ABSTRACT(T) __is_abstract(T)
#	endif // defined(__clang__) && defined(__has_feature)

#	ifdef LIBFTPP_BUILTIN_IS_ABSTRACT

template <typename T>
struct is_abstract : bool_constant<LIBFTPP_BUILTIN_IS_ABSTRACT(T)> {};

#	else // LIBFTPP_BUILTIN_IS_ABSTRACT

/**
 * Fallback implementation.
 * Relies on arrays of abstract class type causing type deduction failure.
 * (DR 337 - https://cplusplus.github.io/CWG/issues/337.html)
 * Inspiration:
 * https://groups.google.com/g/comp.lang.c++.moderated/c/C6gvK2tB_kY/m/ByKtefbsO3UJ
 *
 * However, following DR 1640, the standard was revised with P0929R2 and arrays
 * of abstract class types should not cause type deduction failure anymore.
 * - https://cplusplus.github.io/CWG/issues/1640.html
 * - https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0929r2.html
 *
 * gcc implemented this revision with gcc 11 and therefore this fallback
 * implementation would not work from there on anymore (not a problem because
 * the builtin gets chosen anyway).
 * Discussion: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=17232
 *
 * For clang this implementation would still work until at least version 20.1.0
 * (latest tested).
 */
namespace _is_abstract {
template <typename T, typename = void>
struct impl;
} // namespace _is_abstract

template <typename T>
struct is_abstract : bool_constant<_is_abstract::impl<T>::value> {};

namespace _is_abstract {

template <typename T, typename /*= void*/>
struct impl {
private:
	/**
	 * T must be a complete type. Further, if T is a template then this also
	 * ensures to instantiate it, which is required to get the correct answer.
	 */
	FT_STATIC_ASSERT(ft::is_complete<T>::value); // T must be a complete type.

	template <typename, typename = void>
	struct is_arrayable : false_type {};

	template <typename U>
	struct is_arrayable<U, typename voider<U[1]>::type> : true_type {};

public:
	static const bool value = !is_arrayable<T>::value;
};

/**
 * Filter out types for which sizeof (completeness check) would hard error or
 * which also cannot form arrays.
 */
template <typename T>
struct impl<T,
            typename enable_if<is_function<T>::value || is_reference<T>::value
                               || is_unbounded_array<T>::value
                               || is_void<T>::value>::type> : false_type {};

} // namespace _is_abstract

#	endif // LIBFTPP_BUILTIN_IS_ABSTRACT

/* is_signed */
namespace _is_signed {
template <typename T>
struct impl;
} // namespace _is_signed

template <typename T>
struct is_signed : conjunction<is_arithmetic<T>, _is_signed::impl<T> > {};

namespace _is_signed {

template <typename T>
struct impl : bool_constant<T(-1) < T(0)> {};

} // namespace _is_signed

/* is_unsigned */
namespace _is_unsigned {
template <typename T>
struct impl;
} // namespace _is_unsigned

template <typename T>
struct is_unsigned : conjunction<is_arithmetic<T>, _is_unsigned::impl<T> > {};

namespace _is_unsigned {

template <typename T>
struct impl : bool_constant<T(0) < T(-1)> {};

} // namespace _is_unsigned

/* is_bounded_array */
template <typename>
struct is_bounded_array : false_type {};

template <typename T, std::size_t N>
struct is_bounded_array<T[N]> : true_type {};

/* is_unbounded_array */
template <typename>
struct is_unbounded_array : false_type {};

template <typename T>
struct is_unbounded_array<T[]> : true_type {};

/* Property queries */

/* rank */
template <typename>
struct rank : integral_constant<std::size_t, 0> {};

template <typename T>
struct rank<T[]> : integral_constant<std::size_t, 1 + rank<T>::value> {};

template <typename T, std::size_t N>
struct rank<T[N]> : integral_constant<std::size_t, 1 + rank<T>::value> {};

/* extent */
template <typename, unsigned /*= 0*/>
struct extent : integral_constant<std::size_t, 0> {};

template <typename T, unsigned N>
struct extent<T[], N>
    : integral_constant<std::size_t, N == 0 ? 0 : extent<T, N - 1>::value> {};

template <typename T, std::size_t I, unsigned N>
struct extent<T[I], N>
    : integral_constant<std::size_t, N == 0 ? I : extent<T, N - 1>::value> {};

/* Type relationships */

/* is_same */
template <typename, typename>
struct is_same : false_type {};

template <typename T>
struct is_same<T, T> : true_type {};

/* is_convertible */
#	if defined(__clang__) && defined(__has_feature)
#		if __has_feature(is_convertible_to)
#			define LIBFTPP_BUILTIN_IS_CONVERTIBLE(FROM, TO) \
				__is_convertible_to(FROM, TO)
#		endif // __has_feature(is_convertible_to)
#	elif defined(__GNUC__)                                           \
	    && (__GNUC__ > 13 || (__GNUC__ == 13 && __GNUC_MINOR__ >= 4))
// Buggy 13.1: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=106784
// Fixed 13.4: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=109680
// Related writeup about "Abominable Function Types":
// https://www.open-std.org/JTC1/SC22/WG21/docs/papers/2015/p0172r0.html
#		define LIBFTPP_BUILTIN_IS_CONVERTIBLE(FROM, TO) \
			__is_convertible(FROM, TO)
#	elif defined(_MSC_VER) && _MSC_VER >= 1400
// Appears in Visual Studio 2005 Docs, but not in 2003
#		define LIBFTPP_BUILTIN_IS_CONVERTIBLE(FROM, TO) \
			__is_convertible_to(FROM, TO)
#	endif // defined(__clang__) && defined(__has_feature)

#	ifdef LIBFTPP_BUILTIN_IS_CONVERTIBLE

template <typename From, typename To>
struct is_convertible
    : bool_constant<LIBFTPP_BUILTIN_IS_CONVERTIBLE(From, To)> {};

#	else // LIBFTPP_BUILTIN_IS_CONVERTIBLE

namespace _is_convertible {
template <typename From, typename To, typename = void>
struct impl;
} // namespace _is_convertible

template <typename From, typename To>
struct is_convertible : bool_constant<_is_convertible::impl<From, To>::value> {
};

template <typename From>
struct is_convertible<From, void> : false_type {};

template <typename To>
struct is_convertible<void, To> : false_type {};

template <>
struct is_convertible<void, void> : true_type {};

namespace _is_convertible {

template <typename From>
struct is_impossible_source;
template <typename To>
struct is_impossible_target;
template <typename From, typename To>
struct is_impossible_reference;

/**
 * Uses SFINAE with function overload resolution to determine if a value of type
 * `From` can be implicitly converted to `To`.
 */
template <typename From, typename To, typename /*= void*/>
struct impl {
private:
	/**
	 * Types that cannot be returned by value are returned by reference instead
	 * to ensure the expression is well-formed. Conversions where this would
	 * affect the result are handled in a separate specialization.
	 */
	static typename add_lvalue_reference<From>::type make_from();
	static yes_type can_convert(To);
	static no_type can_convert(...);

public:
	// NOLINTNEXTLINE(bugprone-throwing-static-initialization): Compile-time.
	static const bool value =
	    sizeof(can_convert(make_from())) == sizeof(yes_type);
};

/**
 * Certain conversions are known to be impossible. They are separate because
 * they are not SFINAE-friendly and would otherwise cause compilation errors or
 * incorrect results.
 */
template <typename From, typename To>
struct impl<
    From,
    To,
    typename enable_if<is_impossible_source<From>::value
                       || is_impossible_target<To>::value
                       || is_impossible_reference<From, To>::value>::type>
    : false_type {};

// Example: `is_convertible<int() const, int (*)()>`
template <typename From>
struct is_impossible_source
    : bool_constant<
          is_function<From>::value
          && is_same<typename add_lvalue_reference<From>::type, From>::value> {
};

// Example: `is_convertible<Abstract, Abstract>`
template <typename To>
struct is_impossible_target
    : bool_constant<is_array<To>::value || is_function<To>::value
                    || is_abstract<To>::value> {};

// Example: `is_convertible<Foo, Foo&>`
template <typename From, typename To>
struct is_impossible_reference
    : bool_constant<is_object<From>::value
                    && is_nonconst_lvalue_reference<To>::value> {};

} // namespace _is_convertible

#	endif // LIBFTPP_BUILTIN_IS_CONVERTIBLE

/* Type transformations */

/* remove_cv */
template <typename T>
struct remove_cv
    : type_identity<
          typename remove_volatile<typename remove_const<T>::type>::type> {};

/* remove_const */
template <typename T>
struct remove_const : type_identity<T> {};

template <typename T>
struct remove_const<const T> : type_identity<T> {};

/* remove_volatile */
template <typename T>
struct remove_volatile : type_identity<T> {};

template <typename T>
struct remove_volatile<volatile T> : type_identity<T> {};

/* add_cv */
template <typename T>
struct add_cv
    : type_identity<typename add_volatile<typename add_const<T>::type>::type> {
};

/* add_const */
template <typename T>
struct add_const : type_identity<const T> {};

/* add_volatile */
template <typename T>
struct add_volatile : type_identity<volatile T> {};

/* remove_reference */
template <typename T>
struct remove_reference : type_identity<T> {};

template <typename T>
struct remove_reference<T&> : type_identity<T> {};

template <typename T>
struct remove_reference<ft::rvalue<T>&> : remove_reference<T> {};

template <typename T>
struct remove_reference<ft::rvalue<T&>&> : remove_reference<T&> {};

template <typename T>
struct remove_reference<const ft::rvalue<T>&> : remove_reference<const T> {};

template <typename T>
struct remove_reference<const ft::rvalue<T&>&> : remove_reference<const T&> {};

template <typename T>
struct remove_reference<volatile ft::rvalue<T>&>
    : remove_reference<volatile T> {};

template <typename T>
struct remove_reference<volatile ft::rvalue<T&>&>
    : remove_reference<volatile T&> {};

template <typename T>
struct remove_reference<const volatile ft::rvalue<T>&>
    : remove_reference<const volatile T> {};

template <typename T>
struct remove_reference<const volatile ft::rvalue<T&>&>
    : remove_reference<const volatile T&> {};

/* add_lvalue_reference */
namespace _add_lvalue_reference {
template <typename T, typename = void>
struct impl;
} // namespace _add_lvalue_reference

template <typename T>
struct add_lvalue_reference : _add_lvalue_reference::impl<T> {};

namespace _add_lvalue_reference {

/**
 * void -> void
 * T& -> T&
 */
template <typename T, typename /*= void*/>
struct impl : type_identity<T> {};

/**
 * T -> T&
 * const T -> const T&
 */
template <typename T>
struct impl<T,
            typename voider<T&,
                            typename enable_if<
                                !_type_traits::is_rvalue<T>::value
                                && !is_rvalue_reference<T>::value>::type>::type>
    : type_identity<T&> {};

/**
 * rvalue<T> -> T&
 * rvalue<T>& -> T&
 * rvalue<T&>& -> T&
 * rvalue<rvalue<T> >& -> T&
 * const rvalue<T> -> const T&
 * const rvalue<volatile T> -> const volatile T&
 */
template <typename T>
struct impl<T, typename enable_if<_type_traits::is_rvalue<T>::value>::type>
    : impl<T&> {};

template <typename T>
struct impl<ft::rvalue<T>&, typename voider<T&>::type> : impl<T&> {};

template <typename T>
struct impl<ft::rvalue<T&>&, typename voider<T&>::type> : impl<T&> {};

template <typename T>
struct impl<const ft::rvalue<T>&, typename voider<T&>::type> : impl<const T&> {
};

template <typename T>
struct impl<const ft::rvalue<T&>&, typename voider<T&>::type> : impl<const T&> {
};

template <typename T>
struct impl<volatile ft::rvalue<T>&, typename voider<T&>::type>
    : impl<volatile T&> {};

template <typename T>
struct impl<volatile ft::rvalue<T&>&, typename voider<T&>::type>
    : impl<volatile T&> {};

template <typename T>
struct impl<const volatile ft::rvalue<T>&, typename voider<T&>::type>
    : impl<const volatile T&> {};

template <typename T>
struct impl<const volatile ft::rvalue<T&>&, typename voider<T&>::type>
    : impl<const volatile T&> {};

} // namespace _add_lvalue_reference

/* add_rvalue_reference */
namespace _add_rvalue_reference {
template <typename T,
          typename RemoveCv = typename remove_cv<T>::type,
          bool IsReference = is_reference<T>::value,
          bool IsRvalue = _type_traits::is_rvalue<T>::value,
          typename = void>
struct impl;
} // namespace _add_rvalue_reference

template <typename T>
struct add_rvalue_reference : _add_rvalue_reference::impl<T> {};

namespace _add_rvalue_reference {

/**
 * T& -> T&
 * rvalue<T>& -> rvalue<T>&
 */
template <typename T,
          typename /*= typename remove_cv<T>::type*/,
          bool /*= is_reference<T>::value*/,
          bool /*= _type_traits::is_rvalue<T>::value*/,
          typename /*= void*/>
struct impl : type_identity<T> {};

/**
 * rvalue<T> -> rvalue<T>&
 */
template <typename T, typename RemoveCv>
struct impl<T,
            RemoveCv,
            false, // IsReference
            true,  // IsRvalue
            typename voider<T&>::type> : type_identity<T&> {};

/**
 * T -> rvalue<T>&
 * const T -> const rvalue<T>&
 */
template <typename T, typename RemoveCv>
struct impl<T,
            RemoveCv,
            false, // IsReference
            false, // IsRvalue
            typename voider<T&, ft::rvalue<RemoveCv>&>::type>
    : conditional<
          is_const<T>::value && is_volatile<T>::value,
          const volatile ft::rvalue<RemoveCv>&,
          typename conditional<
              is_const<T>::value,
              const ft::rvalue<RemoveCv>&,
              typename conditional<is_volatile<T>::value,
                                   volatile ft::rvalue<RemoveCv>&,
                                   ft::rvalue<RemoveCv>&>::type>::type> {};

} // namespace _add_rvalue_reference

/* remove_extent */
template <typename T>
struct remove_extent : type_identity<T> {};

template <typename T>
struct remove_extent<T[]> : type_identity<T> {};

template <typename T, std::size_t N>
struct remove_extent<T[N]> : type_identity<T> {};

/* remove_all_extents */
template <typename T>
struct remove_all_extents : type_identity<T> {};

template <typename T>
struct remove_all_extents<T[]> : remove_all_extents<T> {};

template <typename T, std::size_t N>
struct remove_all_extents<T[N]> : remove_all_extents<T> {};

/* remove_pointer */
template <typename T>
struct remove_pointer : conditional<is_pointer<T>::value,
                                    remove_pointer<typename remove_cv<T>::type>,
                                    type_identity<T> >::type {};

template <typename T>
struct remove_pointer<T*> : type_identity<T> {};

/* add_pointer */
namespace _add_pointer {
template <typename T, typename = void>
struct impl;
} // namespace _add_pointer

template <typename T>
struct add_pointer : _add_pointer::impl<T> {};

namespace _add_pointer {

template <typename T, typename /*= void*/>
struct impl : type_identity<T> {};

template <typename T>
struct impl<T, typename voider<typename remove_reference<T>::type*>::type>
    : type_identity<typename remove_reference<T>::type*> {};

} // namespace _add_pointer

/* remove_cvref */
template <typename T>
struct remove_cvref : remove_cv<typename remove_reference<T>::type> {};

/* enable_if */
template <bool, typename T /*= void*/>
struct enable_if : type_identity<T> {};

template <typename T>
struct enable_if<false, T> {};

/* conditional */
template <bool, typename T, typename>
struct conditional : type_identity<T> {};

template <typename T, typename F>
struct conditional<false, T, F> : type_identity<F> {};

/* voider (void_t) */
template <typename /*= void*/,
          typename /*= void*/,
          typename /*= void*/,
          typename /*= void*/,
          typename /*= void*/,
          typename /*= void*/,
          typename /*= void*/,
          typename /*= void*/,
          typename /*= void*/,
          typename /*= void*/>
struct voider : type_identity<void> {};

/* Logical operations */

/* conjunction */
template <typename B1 /*= _type_traits::null_type*/,
          typename B2 /*= _type_traits::null_type*/,
          typename B3 /*= _type_traits::null_type*/,
          typename B4 /*= _type_traits::null_type*/,
          typename B5 /*= _type_traits::null_type*/,
          typename B6 /*= _type_traits::null_type*/,
          typename B7 /*= _type_traits::null_type*/,
          typename B8 /*= _type_traits::null_type*/,
          typename B9 /*= _type_traits::null_type*/,
          typename B10 /*= _type_traits::null_type*/>
struct conjunction
    : conditional<!bool(B1::value)
                      || is_same<B2, _type_traits::null_type>::value,
                  B1,
                  conjunction<B2,
                              B3,
                              B4,
                              B5,
                              B6,
                              B7,
                              B8,
                              B9,
                              B10,
                              _type_traits::null_type> >::type {};

template <>
struct conjunction<_type_traits::null_type> : true_type {};

/* disjunction */
template <typename B1 /*= _type_traits::null_type*/,
          typename B2 /*= _type_traits::null_type*/,
          typename B3 /*= _type_traits::null_type*/,
          typename B4 /*= _type_traits::null_type*/,
          typename B5 /*= _type_traits::null_type*/,
          typename B6 /*= _type_traits::null_type*/,
          typename B7 /*= _type_traits::null_type*/,
          typename B8 /*= _type_traits::null_type*/,
          typename B9 /*= _type_traits::null_type*/,
          typename B10 /*= _type_traits::null_type*/>
struct disjunction
    : conditional<bool(B1::value)
                      || is_same<B2, _type_traits::null_type>::value,
                  B1,
                  disjunction<B2,
                              B3,
                              B4,
                              B5,
                              B6,
                              B7,
                              B8,
                              B9,
                              B10,
                              _type_traits::null_type> >::type {};

template <>
struct disjunction<_type_traits::null_type> : false_type {};

/* negation */
template <typename B>
struct negation : bool_constant<!bool(B::value)> {};

/* Custom type traits */

/* is_class_or_union */
namespace _is_class_or_union {
template <typename T, typename = void>
struct impl;
} // namespace _is_class_or_union

template <typename T>
struct is_class_or_union : _is_class_or_union::impl<T> {};

namespace _is_class_or_union {

template <typename, typename /*= void*/>
struct impl : false_type {};

template <typename T>
struct impl<T, typename voider<int T::*>::type> : true_type {};

} // namespace _is_class_or_union

/* is_complete */
namespace _is_complete {
template <typename T, typename = void>
struct impl;
} // namespace _is_complete

template <typename T>
struct is_complete : _is_complete::impl<T> {};

namespace _is_complete {

template <typename, typename /*= void*/>
struct impl : false_type {};

template <typename T>
struct impl<T, typename enable_if<sizeof(T) != 0>::type> : true_type {};

} // namespace _is_complete

/* is_const_lvalue_reference */
/**
 * If `T` is a reference type then `is_const<T>::value` is always `false`. The
 * proper way to check a potentially-reference type for constness is to remove
 * the reference first.
 */
template <typename T>
struct is_const_lvalue_reference
    : bool_constant<is_lvalue_reference<T>::value
                    && is_const<typename remove_reference<T>::type>::value> {};

/* is_nonconst_lvalue_reference */
/**
 * If `T` is a reference type then `!is_const<T>::value` is always `true`. The
 * proper way to check a potentially-reference type for constness is to remove
 * the reference first.
 */
template <typename T>
struct is_nonconst_lvalue_reference
    : bool_constant<is_lvalue_reference<T>::value
                    && !is_const<typename remove_reference<T>::type>::value> {};

/* is_returnable */
/**
 * Because of DR 1646, a lot of compiler versions before P0929R2 behave
 * differently on function declarations and template substitutions. Therefore,
 * using deduction failure to implement this trait would not be reliable.
 * https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0929r2.html
 */
template <typename T>
struct is_returnable
    : bool_constant<!is_abstract<T>::value && !is_array<T>::value
                    && !is_function<T>::value> {};

} // namespace ft

#endif // LIBFTPP_TYPE_TRAITS_TYPE_TRAITS_TPP
