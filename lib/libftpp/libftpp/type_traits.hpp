#pragma once
#ifndef LIBFTPP_TYPE_TRAITS_HPP
#	define LIBFTPP_TYPE_TRAITS_HPP

#	include "libftpp/type_traits/has_member_function.ipp"

/**
 * @brief Generates type traits that check if a class has a specific public
 * non-static member function.
 *
 * The generated type traits are named has_member_function_<name>.
 * There can only be 1 trait with the same function name.
 *
 * Usage example: `FT_HAS_MEMBER_FUNCTION(void, swap, (T&))`
 */
#	define FT_HAS_MEMBER_FUNCTION(RETURN_TYPE, NAME, ARGS_IN_PARENS)       \
		LIBFTPP_HAS_MEMBER_FUNCTION_IMPL(RETURN_TYPE, NAME, ARGS_IN_PARENS)

/**
 * @brief A macro that emulates C++20's `requires` clauses in C++98
 *
 * `FT_REQUIRES` is used for function template overload resolution. It enables a
 * function template if and only if the provided compile-time `EXPRESSION`
 * evaluates to `true` when explicitly converted to `bool`.
 * The function's return type must be placed in parentheses after the macro.
 *
 * The goal of this macro is to make function signatures that would otherwise
 * use `enable_if` more readable.
 *
 * Usage examples:
 * ```
 * template <typename T>
 * FT_REQUIRES((ft::is_integral<T>::value && !ft::is_same<T, int>::value))
 * (T) function(T x);
 * ```
 * Note the double parentheses around the expression to escape the comma.
 *
 * ```
 * template <typename T, unsigned N>
 * FT_REQUIRES(N < 10)
 * ((std::pair<T, T>)) function(T x);
 * ```
 * Note the double parentheses around the return type to escape the comma.
 *
 * Inspiration: https://stackoverflow.com/a/9220563
 *
 * `clang-format`: To help `clang-format` format code using this macro better,
 * add `FT_REQUIRES` to `StatementMacros` in the `clang-format` config.
 */
#	define FT_REQUIRES(EXPRESSION) LIBFTPP_REQUIRES_IMPL(EXPRESSION)

namespace ft {

/**
 * @brief Helper types with distinct sizes for SFINAE `sizeof` tricks
 *
 * These types are commonly used in template metaprogramming as return types in
 * overloaded functions to determine which overload was selected at compile
 * time. An expression that resolves to one of these types is evaluated with
 * `sizeof`, and the result is compared against `sizeof(yes_type)` to produce a
 * compile-time boolean value.
 */
typedef char (&yes_type)[1];
typedef char (&no_type)[2];

/* Base classes */

/**
 * https://en.cppreference.com/w/cpp/types/integral_constant
 */
template <typename T, T v>
struct integral_constant {
	static const T value = v;
	typedef T value_type;
	typedef integral_constant type;

	// Original in `std::integral_constant` is not `explicit`.
	operator value_type() const throw(); // NOLINT(google-explicit-constructor)
	value_type operator()() const throw();
};

template <bool B>
struct bool_constant : integral_constant<bool, B> {};

typedef bool_constant<true> true_type;
typedef bool_constant<false> false_type;

/**
 * https://en.cppreference.com/w/cpp/types/type_identity
 */
template <typename T>
struct type_identity {
	typedef T type;
};

/* Unary type traits */

/**
 * https://en.cppreference.com/w/cpp/types/is_void
 */
template <typename T>
struct is_void;

/**
 * https://en.cppreference.com/w/cpp/types/is_integral
 */
template <typename T>
struct is_integral;

/**
 * https://en.cppreference.com/w/cpp/types/is_floating_point
 */
template <typename T>
struct is_floating_point;

/**
 * https://en.cppreference.com/w/cpp/types/is_array
 */
template <typename T>
struct is_array;

/**
 * https://en.cppreference.com/w/cpp/types/is_function
 */
template <typename T>
struct is_function;

/**
 * https://en.cppreference.com/w/cpp/types/is_pointer
 */
template <typename T>
struct is_pointer;

/**
 * https://en.cppreference.com/w/cpp/types/is_lvalue_reference
 */
template <typename T>
struct is_lvalue_reference;

/**
 * https://en.cppreference.com/w/cpp/types/is_rvalue_reference
 */
template <typename T>
struct is_rvalue_reference;

/**
 * https://en.cppreference.com/w/cpp/types/is_arithmetic
 */
template <typename T>
struct is_arithmetic;

/**
 * https://en.cppreference.com/w/cpp/types/is_object
 */
template <typename T>
struct is_object;

/**
 * https://en.cppreference.com/w/cpp/types/is_reference
 */
template <typename T>
struct is_reference;

/**
 * https://en.cppreference.com/w/cpp/types/is_const
 */
template <typename T>
struct is_const;

/**
 * https://en.cppreference.com/w/cpp/types/is_volatile
 */
template <typename T>
struct is_volatile;

/**
 * https://en.cppreference.com/w/cpp/types/is_abstract
 */
template <typename T>
struct is_abstract;

/**
 * https://en.cppreference.com/w/cpp/types/is_signed
 */
template <typename T>
struct is_signed;

/**
 * https://en.cppreference.com/w/cpp/types/is_unsigned
 */
template <typename T>
struct is_unsigned;

/**
 * https://en.cppreference.com/w/cpp/types/is_bounded_array
 */
template <typename T>
struct is_bounded_array;

/**
 * https://en.cppreference.com/w/cpp/types/is_unbounded_array
 */
template <typename T>
struct is_unbounded_array;

/* Property queries */

/**
 * https://en.cppreference.com/w/cpp/types/rank
 */
template <typename T>
struct rank;

/**
 * https://en.cppreference.com/w/cpp/types/extent
 */
template <typename T, unsigned N = 0>
struct extent;

/* Type relationships */

/**
 * https://en.cppreference.com/w/cpp/types/is_same
 */
template <typename T, typename U>
struct is_same;

/**
 * https://en.cppreference.com/w/cpp/types/is_convertible
 */
template <typename From, typename To>
struct is_convertible;

/* Type transformations */

/**
 * https://en.cppreference.com/w/cpp/types/remove_cv
 */
template <typename T>
struct remove_cv;

/**
 * https://en.cppreference.com/w/cpp/types/remove_cv
 */
template <typename T>
struct remove_const;

/**
 * https://en.cppreference.com/w/cpp/types/remove_cv
 */
template <typename T>
struct remove_volatile;

/**
 * https://en.cppreference.com/w/cpp/types/add_cv
 */
template <typename T>
struct add_cv;

/**
 * https://en.cppreference.com/w/cpp/types/add_cv
 */
template <typename T>
struct add_const;

/**
 * https://en.cppreference.com/w/cpp/types/add_cv
 */
template <typename T>
struct add_volatile;

/**
 * https://en.cppreference.com/w/cpp/types/remove_reference
 */
template <typename T>
struct remove_reference;

/**
 * https://en.cppreference.com/w/cpp/types/add_reference
 */
template <typename T>
struct add_lvalue_reference;

/**
 * https://en.cppreference.com/w/cpp/types/add_reference
 */
template <typename T>
struct add_rvalue_reference;

/**
 * https://en.cppreference.com/w/cpp/types/remove_extent
 */
template <typename T>
struct remove_extent;

/**
 * https://en.cppreference.com/w/cpp/types/remove_all_extents
 */
template <typename T>
struct remove_all_extents;

/**
 * https://en.cppreference.com/w/cpp/types/remove_pointer
 */
template <typename T>
struct remove_pointer;

/**
 * https://en.cppreference.com/w/cpp/types/add_pointer
 */
template <typename T>
struct add_pointer;

/**
 * https://en.cppreference.com/w/cpp/types/remove_cvref
 */
template <typename T>
struct remove_cvref;

/**
 * https://en.cppreference.com/w/cpp/types/enable_if
 */
template <bool B, typename T = void>
struct enable_if;

/**
 * https://en.cppreference.com/w/cpp/types/conditional
 */
template <bool B, typename T, typename F>
struct conditional;

/**
 * https://en.cppreference.com/w/cpp/types/void_t
 *
 * Variadic templates do not exist in C++98, so an arbitrary limit of 10
 * template parameters is set instead.
 *
 * It is not named `void_t` because `::type` is still required to use it.
 * The `_t` suffix is usually used for helper types that don't need `::type`.
 * The name `voider` is chosen because it is mentioned in N3911
 * (https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n3911.pdf).
 */
template <typename = void,
          typename = void,
          typename = void,
          typename = void,
          typename = void,
          typename = void,
          typename = void,
          typename = void,
          typename = void,
          typename = void>
struct voider;

/* Logical operations */

/**
 * https://en.cppreference.com/w/cpp/types/conjunction
 *
 * Variadic templates do not exist in C++98, so an arbitrary limit of 10
 * template parameters is set instead.
 */
template <typename B1 /* optional */,
          typename B2 /* optional */,
          typename B3 /* optional */,
          typename B4 /* optional */,
          typename B5 /* optional */,
          typename B6 /* optional */,
          typename B7 /* optional */,
          typename B8 /* optional */,
          typename B9 /* optional */,
          typename B10 /* optional */>
struct conjunction;

/**
 * https://en.cppreference.com/w/cpp/types/disjunction
 *
 * Variadic templates do not exist in C++98, so an arbitrary limit of 10
 * template parameters is set instead.
 */
template <typename B1 /* optional */,
          typename B2 /* optional */,
          typename B3 /* optional */,
          typename B4 /* optional */,
          typename B5 /* optional */,
          typename B6 /* optional */,
          typename B7 /* optional */,
          typename B8 /* optional */,
          typename B9 /* optional */,
          typename B10 /* optional */>
struct disjunction;

/**
 * https://en.cppreference.com/w/cpp/types/negation
 */
template <typename B>
struct negation;

/* Custom type traits */

/* has_member_function_swap */
FT_HAS_MEMBER_FUNCTION(void, swap, (T&))

/**
 * @brief Checks whether `T` is a class or union type
 *
 * Provides the member constant `value` which is equal to `true`, if `T` is a
 * class or union type. Otherwise, `value` is equal to `false`.
 *
 * Classes and unions cannot be differentiated without compiler builtins, so
 * this is a portable compromise.
 */
template <typename T>
struct is_class_or_union;

/**
 * @brief Checks whether `T` is a complete type
 *
 * Provides the member constant `value` which is equal to `true`, if `T` is a
 * complete type at the point of instantiation of this trait. Otherwise, `value`
 * is equal to `false`.
 *
 * The following types are incomplete types:
 * - the type void (possibly cv-qualified);
 * - incompletely-defined object types:
 *   - class type that has been declared (e.g. by forward declaration) but not
 *     defined;
 *   - array of unknown bound;
 *   - array of elements of incomplete type;
 *   - enumeration type from the point of declaration until its underlying type
 *     is determined.
 */
template <typename T>
struct is_complete;

/**
 * @brief Checks whether `T` is an lvalue reference to a const-qualified type
 *
 * Provides the member constant `value` which is equal to `true`, if `T` is an
 * lvalue reference to a const-qualified type. Otherwise, `value` is equal to
 * `false`.
 *
 * This is useful because if `T` is a reference type then `is_const<T>::value`
 * is always `false`.
 */
template <typename T>
struct is_const_lvalue_reference;

/**
 * @brief Checks whether `T` is an lvalue reference to a non-const-qualified
 * type
 *
 * Provides the member constant `value` which is equal to `true`, if `T` is an
 * lvalue reference to a non-const-qualified type. Otherwise, `value` is equal
 * to `false`.
 *
 * This is useful because if `T` is a reference type then `!is_const<T>::value`
 * is always `true`.
 */
template <typename T>
struct is_nonconst_lvalue_reference;

/**
 * @brief Checks whether `T` can be a return type of a function
 *
 * Provides the member constant `value` which is equal to `true`, if `T` is not
 * an abstract class, an array, or a function. Otherwise, `value` is equal to
 * `false`.
 *
 * This trait reflects the type constraints of a function's definition
 * or call, not just its declaration.
 */
template <typename T>
struct is_returnable;

} // namespace ft

#	include "libftpp/type_traits/requires.ipp"    // IWYU pragma: keep
#	include "libftpp/type_traits/type_traits.tpp" // IWYU pragma: export

#endif // LIBFTPP_TYPE_TRAITS_HPP
