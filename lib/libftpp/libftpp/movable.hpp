/**
 * Move semantics in C++98
 *
 * Inspiration: https://www.boost.org/doc/libs/1_84_0/doc/html/move.html
 * Limitations:
 * https://www.boost.org/doc/libs/1_84_0/doc/html/move/emulation_limitations.html
 */

#pragma once
#ifndef LIBFTPP_MOVABLE_HPP
#	define LIBFTPP_MOVABLE_HPP

#	include "libftpp/movable/movable.ipp"
#	include "libftpp/movable/movable_detail.hpp"

/**
 * @brief Make a type copyable and movable
 *
 * Usage:
 * 1. Put the following macro in the **private** section:
 *   `FT_COPYABLE_AND_MOVABLE(YourClass)`.
 * 2. Leave copy constructor as is.
 * 3. Write a copy assignment as usual but take the parameter as
 *    `const ft::copy_assign_ref<YourClass>&`.
 * 4. Write a move constructor and a move assignment taking the parameter as
 *    `ft::rvalue<YourClass>&`.
 *
 * To use the copy-and-swap idiom, see `FT_COPYABLE_AND_MOVABLE_SWAP`.
 *
 * https://www.boost.org/doc/libs/1_84_0/doc/html/move/implementing_movable_classes.html
 *
 * @note Do **not** declare move constructors `explicit`.
 *
 * @note Implicit move operations when returning a local object are not
 * supported. A call to `ft::move()` is required.
 *
 * @see FT_COPYABLE_AND_MOVABLE_SWAP, FT_MOVABLE_BUT_NOT_COPYABLE
 */
#	define FT_COPYABLE_AND_MOVABLE(TYPE)       \
		LIBFTPP_COPYABLE_AND_MOVABLE_IMPL(TYPE)

/**
 * @brief Make a type copyable and movable using the copy-and-swap idiom
 *
 * Instead of writing a copy assignment operator and a move assignment
 * operator, you can also write a single unifying assignment operator taking
 * `YourClass` by value and use the copy-and-swap idiom.
 * https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Copy-and-swap
 *
 * Usage:
 * 1. Put the following macro in the **private** section:
 *   `FT_COPYABLE_AND_MOVABLE_SWAP(YourClass)`.
 * 2. Leave copy constructor as is.
 * 3. Write a move constructor taking the parameter as `ft::rvalue<YourClass>&`.
 * 4. Write a unifying assignment operator taking the parameter by value as
 *    `YourClass` and use the copy-and-swap idiom.
 *
 * @note Do **not** declare move constructors `explicit`.
 *
 * @note Implicit move operations when returning a local object are not
 * supported. A call to `ft::move()` is required.
 *
 * @see FT_COPYABLE_AND_MOVABLE, FT_MOVABLE_BUT_NOT_COPYABLE
 */
#	define FT_COPYABLE_AND_MOVABLE_SWAP(TYPE)       \
		LIBFTPP_COPYABLE_AND_MOVABLE_SWAP_IMPL(TYPE)

/**
 * @brief Make a type movable but not copyable
 *
 * Usage:
 * 1. Put the following macro in the **private** section:
 *    `FT_MOVABLE_BUT_NOT_COPYABLE(YourClass)`.
 * 2. Write a move constructor and a move assignment taking the parameter as
 *    `ft::rvalue<YourClass>&`.
 *
 * https://www.boost.org/doc/libs/1_84_0/doc/html/move/movable_only_classes.html
 *
 * @note Do **not** disable the copy constructor and copy assignment operator
 * manually.
 *
 * @note Do **not** declare move constructors `explicit`.
 *
 * @note Implicit move operations when returning a local object are not
 * supported. A call to `ft::move()` is required.
 *
 * @see FT_COPYABLE_AND_MOVABLE, FT_COPYABLE_AND_MOVABLE_SWAP
 */
#	define FT_MOVABLE_BUT_NOT_COPYABLE(TYPE)       \
		LIBFTPP_MOVABLE_BUT_NOT_COPYABLE_IMPL(TYPE)

namespace ft {

/**
 * @brief Copy assignment reference wrapper used with
 * `FT_COPYABLE_AND_MOVABLE()`
 *
 * Needed to make a type copyable and movable with `FT_COPYABLE_AND_MOVABLE()`.
 * It is a replacement for taking the copy assignment operator parameter as
 * `const YourClass&` to avoid it having too high priority by overload
 * resolution over rvalues.
 * Use only as `const ft::copy_assign_ref<YourClass>&` for copy assignment
 * operators and in combination with `FT_COPYABLE_AND_MOVABLE()`.
 */
template <typename T>
struct copy_assign_ref : public _movable::inherit_if_class_or_union<T> {
private:
	copy_assign_ref();
	copy_assign_ref(const copy_assign_ref&);
	void operator=(const copy_assign_ref&);
	~copy_assign_ref();
};

/**
 * @brief Rvalue reference emulation in C++98
 *
 * A function taking a parameter of type `rvalue<T>&` will be chosen by
 * overload resolution if `T` implements the move semantics of libftpp and the
 * argument is an rvalue (temporary object or `ft::move()` cast).
 *
 * https://www.boost.org/doc/libs/1_84_0/doc/html/move/how_the_library_works.html
 *
 * @note `T` must be a class type.
 */
template <typename T>
struct rvalue : public copy_assign_ref<T> {
private:
	rvalue();
	rvalue(const rvalue&);
	void operator=(const rvalue&);
	~rvalue();
};

} // namespace ft

#endif // LIBFTPP_MOVABLE_HPP
