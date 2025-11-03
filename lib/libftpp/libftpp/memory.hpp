#pragma once
#ifndef LIBFTPP_MEMORY_HPP
#	define LIBFTPP_MEMORY_HPP

#	include "libftpp/memory/shared_ptr_detail.hpp"
#	include "libftpp/memory/unique_ptr_detail.hpp"
#	include "libftpp/assert.hpp"
#	include "libftpp/movable.hpp"
#	include "libftpp/safe_bool.hpp"
#	include "libftpp/type_traits.hpp"
#	include "libftpp/utility.hpp"
#	include <cstddef>
#	include <memory>
#	include <ostream>

namespace ft {

/* default_delete */

/**
 * https://en.cppreference.com/w/cpp/memory/default_delete
 */
template <typename T>
struct default_delete {
private:
	struct _enabler {};

public:
	default_delete() throw();
	// NOLINTBEGIN(google-explicit-constructor): Original in
	// `std::default_delete` is not `explicit`.
	template <typename U>
	default_delete(const default_delete<U>& d,
	               typename ft::enable_if<ft::is_convertible<U*, T*>::value,
	                                      _enabler>::type /*unused*/
	               = _enabler()) throw();
	// NOLINTEND(google-explicit-constructor)

	void operator()(T* ptr) const;
};

/**
 * https://en.cppreference.com/w/cpp/memory/default_delete
 */
template <typename T>
struct default_delete<T[]> {
private:
	struct _enabler {};

public:
	default_delete() throw();
	// NOLINTBEGIN(google-explicit-constructor): Original in
	// `std::default_delete` is not `explicit`.
	template <typename U>
	default_delete(
	    const default_delete<U[]>& d,
	    typename ft::enable_if<ft::is_convertible<U (*)[], T (*)[]>::value,
	                           _enabler>::type /*unused*/
	    = _enabler()) throw();
	// NOLINTEND(google-explicit-constructor)

	template <typename U>
	FT_REQUIRES((ft::is_convertible<U (*)[], T (*)[]>::value))
	(void)operator()(U * ptr) const;
};

/* unique_ptr */

/**
 * https://en.cppreference.com/w/cpp/memory/unique_ptr
 *
 * @note Don't create a unique_ptr from a pointer unless you know where the
 * pointer came from and that it needs an owner. This prevents double frees.
 */
template <typename T, typename Deleter = default_delete<T> >
class unique_ptr : public ft::safe_bool<unique_ptr<T, Deleter> > {
private:
	FT_STATIC_ASSERT(ft::is_pointer<T*>::value); // T* must form a valid type.

	template <typename U, typename E>
	struct _is_compatible_unique_ptr;
	struct _enabler {};

public:
	/**
	 * `remove_reference<Deleter>::type::pointer` if that type exists, otherwise
	 * `T*`.
	 */
	typedef typename _unique_ptr::pointer<T, Deleter>::type pointer;
	typedef T element_type;
	typedef Deleter deleter_type;

	// 1)
	unique_ptr() throw();
	// NOLINTBEGIN(google-explicit-constructor): Original in `std::unique_ptr`
	// is not `explicit`.
	unique_ptr(ft::nullptr_t /*unused*/) throw();
	// NOLINTEND(google-explicit-constructor)
	// 2)
	template <typename Pointer>
	explicit unique_ptr(
	    Pointer p,
	    typename ft::enable_if<ft::is_convertible<Pointer, pointer>::value
	                               && !ft::is_pointer<Deleter>::value,
	                           _enabler>::type /*unused*/
	    = _enabler()) throw();
	// 3) a,b,c) (1)
	unique_ptr(pointer p,
	           typename ft::conditional<
	               !ft::is_reference<Deleter>::value,
	               typename ft::add_lvalue_reference<const Deleter>::type,
	               Deleter>::type d) throw();
	// 4) a) (2)
	template <typename Pointer>
	unique_ptr(
	    Pointer p,
	    ft::rvalue<Deleter>& d,
	    typename ft::enable_if<ft::is_convertible<Pointer, pointer>::value
	                               && !ft::is_reference<Deleter>::value,
	                           _enabler>::type /*unused*/
	    = _enabler()) throw();
	// NOLINTBEGIN(google-explicit-constructor): Move constructors.
	// 5)
	unique_ptr(ft::rvalue<unique_ptr>& u) throw();
	// 6)
	template <typename U, typename E>
	unique_ptr(ft::rvalue<unique_ptr<U, E> >& u,
	           typename ft::enable_if<
	               _is_compatible_unique_ptr<U, E>::value
	                   && _unique_ptr::is_compatible_deleter<E, Deleter>::value,
	               _enabler>::type /*unused*/
	           = _enabler()) throw();
#	if __cplusplus <= 201402L
	// 8)
	template <typename U>
	unique_ptr(ft::rvalue<std::auto_ptr<U> >& u,
	           typename ft::enable_if<
	               ft::is_convertible<U*, T*>::value
	                   && ft::is_same<Deleter, default_delete<T> >::value,
	               _enabler>::type /*unused*/
	           = _enabler()) throw();
#	endif // __cplusplus <= 201402L
	// NOLINTEND(google-explicit-constructor)
	~unique_ptr();
	unique_ptr& operator=(ft::rvalue<unique_ptr>& r) throw();
	template <typename U, typename E>
	unique_ptr& operator=(ft::rvalue<unique_ptr<U, E> >& r) throw();
	unique_ptr& operator=(ft::nullptr_t /*unused*/) throw();

	pointer release() throw();
	void reset(pointer ptr = pointer()) throw();
	void swap(unique_ptr& other) throw();

	/**
	 * @note Make sure the returned raw pointer is no longer in use after the
	 * unique_ptr goes out of scope.
	 */
	pointer get() const throw();
	// C++98 did not have reference collapsing yet.
	typename ft::add_lvalue_reference<Deleter>::type get_deleter() throw();
	typename ft::add_lvalue_reference<const Deleter>::type get_deleter() const
	    throw();
	bool operator_bool() const throw();

	typename ft::add_lvalue_reference<T>::type operator*() const;
	pointer operator->() const throw();

private:
	FT_MOVABLE_BUT_NOT_COPYABLE(unique_ptr)

	// 4) b,c) (2)
	template <typename Pointer>
	unique_ptr(
	    Pointer p,
	    typename ft::add_rvalue_reference<
	        typename ft::remove_reference<Deleter>::type>::type d,
	    typename ft::enable_if<ft::is_convertible<Pointer, pointer>::value
	                               && ft::is_lvalue_reference<Deleter>::value,
	                           _enabler>::type = _enabler()) throw();

	pointer _ptr;
	Deleter _deleter;
};

/**
 * https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique
 */
template <typename T>
FT_REQUIRES(!ft::is_array<T>::value)
(unique_ptr<T>)make_unique();
template <typename T, typename A0>
FT_REQUIRES(!ft::is_array<T>::value)
(unique_ptr<T>)make_unique(const A0& arg0);
template <typename T, typename A0, typename A1>
FT_REQUIRES(!ft::is_array<T>::value)
(unique_ptr<T>)make_unique(const A0& arg0, const A1& arg1);
template <typename T, typename A0, typename A1, typename A2>
FT_REQUIRES(!ft::is_array<T>::value)
(unique_ptr<T>)make_unique(const A0& arg0, const A1& arg1, const A2& arg2);
template <typename T, typename A0, typename A1, typename A2, typename A3>
FT_REQUIRES(!ft::is_array<T>::value)
(unique_ptr<T>)
    make_unique(const A0& arg0, const A1& arg1, const A2& arg2, const A3& arg3);
template <typename T,
          typename A0,
          typename A1,
          typename A2,
          typename A3,
          typename A4>
FT_REQUIRES(!ft::is_array<T>::value)
(unique_ptr<T>)make_unique(const A0& arg0,
                           const A1& arg1,
                           const A2& arg2,
                           const A3& arg3,
                           const A4& arg4);
template <typename T,
          typename A0,
          typename A1,
          typename A2,
          typename A3,
          typename A4,
          typename A5>
FT_REQUIRES(!ft::is_array<T>::value)
(unique_ptr<T>)make_unique(const A0& arg0,
                           const A1& arg1,
                           const A2& arg2,
                           const A3& arg3,
                           const A4& arg4,
                           const A5& arg5);
template <typename T,
          typename A0,
          typename A1,
          typename A2,
          typename A3,
          typename A4,
          typename A5,
          typename A6>
FT_REQUIRES(!ft::is_array<T>::value)
(unique_ptr<T>)make_unique(const A0& arg0,
                           const A1& arg1,
                           const A2& arg2,
                           const A3& arg3,
                           const A4& arg4,
                           const A5& arg5,
                           const A6& arg6);
template <typename T,
          typename A0,
          typename A1,
          typename A2,
          typename A3,
          typename A4,
          typename A5,
          typename A6,
          typename A7>
FT_REQUIRES(!ft::is_array<T>::value)
(unique_ptr<T>)make_unique(const A0& arg0,
                           const A1& arg1,
                           const A2& arg2,
                           const A3& arg3,
                           const A4& arg4,
                           const A5& arg5,
                           const A6& arg6,
                           const A7& arg7);
template <typename T,
          typename A0,
          typename A1,
          typename A2,
          typename A3,
          typename A4,
          typename A5,
          typename A6,
          typename A7,
          typename A8>
FT_REQUIRES(!ft::is_array<T>::value)
(unique_ptr<T>)make_unique(const A0& arg0,
                           const A1& arg1,
                           const A2& arg2,
                           const A3& arg3,
                           const A4& arg4,
                           const A5& arg5,
                           const A6& arg6,
                           const A7& arg7,
                           const A8& arg8);
template <typename T,
          typename A0,
          typename A1,
          typename A2,
          typename A3,
          typename A4,
          typename A5,
          typename A6,
          typename A7,
          typename A8,
          typename A9>
FT_REQUIRES(!ft::is_array<T>::value)
(unique_ptr<T>)make_unique(const A0& arg0,
                           const A1& arg1,
                           const A2& arg2,
                           const A3& arg3,
                           const A4& arg4,
                           const A5& arg5,
                           const A6& arg6,
                           const A7& arg7,
                           const A8& arg8,
                           const A9& arg9);

/**
 * https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique
 */
template <typename T>
FT_REQUIRES(!ft::is_array<T>::value)
(unique_ptr<T>)make_unique_for_overwrite();

template <typename T1, typename D1, typename T2, typename D2>
bool operator==(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);
template <typename T1, typename D1, typename T2, typename D2>
bool operator!=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);
template <typename T1, typename D1, typename T2, typename D2>
bool operator<(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);
template <typename T1, typename D1, typename T2, typename D2>
bool operator<=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);
template <typename T1, typename D1, typename T2, typename D2>
bool operator>(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);
template <typename T1, typename D1, typename T2, typename D2>
bool operator>=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y);
template <typename T, typename D>
bool operator==(const unique_ptr<T, D>& x, ft::nullptr_t /*unused*/) throw();
template <typename T, typename D>
bool operator==(ft::nullptr_t /*unused*/, const unique_ptr<T, D>& x) throw();
template <typename T, typename D>
bool operator!=(const unique_ptr<T, D>& x, ft::nullptr_t /*unused*/) throw();
template <typename T, typename D>
bool operator!=(ft::nullptr_t /*unused*/, const unique_ptr<T, D>& x) throw();
template <typename T, typename D>
bool operator<(const unique_ptr<T, D>& x, ft::nullptr_t /*unused*/);
template <typename T, typename D>
bool operator<(ft::nullptr_t /*unused*/, const unique_ptr<T, D>& y);
template <typename T, typename D>
bool operator<=(const unique_ptr<T, D>& x, ft::nullptr_t /*unused*/);
template <typename T, typename D>
bool operator<=(ft::nullptr_t /*unused*/, const unique_ptr<T, D>& y);
template <typename T, typename D>
bool operator>(const unique_ptr<T, D>& x, ft::nullptr_t /*unused*/);
template <typename T, typename D>
bool operator>(ft::nullptr_t /*unused*/, const unique_ptr<T, D>& y);
template <typename T, typename D>
bool operator>=(const unique_ptr<T, D>& x, ft::nullptr_t /*unused*/);
template <typename T, typename D>
bool operator>=(ft::nullptr_t /*unused*/, const unique_ptr<T, D>& y);

template <typename CharT, typename Traits, typename Y, typename D>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const unique_ptr<Y, D>& p);

template <typename T, typename D>
void swap(unique_ptr<T, D>& lhs, unique_ptr<T, D>& rhs) throw();

/**
 * https://en.cppreference.com/w/cpp/memory/unique_ptr
 */
template <typename T, typename Deleter /*= default_delete<T> */>
class unique_ptr<T[], Deleter>
    : public ft::safe_bool<unique_ptr<T[], Deleter> > {
private:
	FT_STATIC_ASSERT(ft::is_pointer<T*>::value); // T* must form a valid type.

	template <typename U>
	struct _is_compatible_raw_pointer;
	template <typename U, typename E>
	struct _is_compatible_unique_ptr;
	struct _enabler {};

public:
	/**
	 * `remove_reference<Deleter>::type::pointer` if that type exists, otherwise
	 * `T*`.
	 */
	typedef typename _unique_ptr::pointer<T, Deleter>::type pointer;
	typedef T element_type;
	typedef Deleter deleter_type;

	// 1)
	unique_ptr() throw();
	// NOLINTBEGIN(google-explicit-constructor): Original in `std::unique_ptr`
	// is not `explicit`.
	unique_ptr(ft::nullptr_t /*unused*/) throw();
	// NOLINTEND(google-explicit-constructor)
	// 2)
	template <typename U>
	explicit unique_ptr(
	    U p,
	    typename ft::enable_if<!ft::is_pointer<Deleter>::value
	                               && _is_compatible_raw_pointer<U>::value,
	                           _enabler>::type /*unused*/
	    = _enabler()) throw();
	// 3) a,b,c) (1)
	template <typename U>
	unique_ptr(U p,
	           typename ft::conditional<
	               !ft::is_reference<Deleter>::value,
	               typename ft::add_lvalue_reference<const Deleter>::type,
	               Deleter>::type d,
	           typename ft::enable_if<_is_compatible_raw_pointer<U>::value,
	                                  _enabler>::type /*unused*/
	           = _enabler()) throw();
	// 4) a) (2)
	template <typename U>
	unique_ptr(
	    U p,
	    ft::rvalue<Deleter>& d,
	    typename ft::enable_if<!ft::is_reference<Deleter>::value
	                               && _is_compatible_raw_pointer<U>::value,
	                           _enabler>::type /*unused*/
	    = _enabler()) throw();
	// NOLINTBEGIN(google-explicit-constructor): Move constructors.
	// 5)
	unique_ptr(ft::rvalue<unique_ptr>& u) throw();
	// 6)
	template <typename U, typename E>
	unique_ptr(ft::rvalue<unique_ptr<U, E> >& u,
	           typename ft::enable_if<
	               _is_compatible_unique_ptr<U, E>::value
	                   && _unique_ptr::is_compatible_deleter<E, Deleter>::value,
	               _enabler>::type /*unused*/
	           = _enabler()) throw();
	// NOLINTEND(google-explicit-constructor)
	~unique_ptr();
	unique_ptr& operator=(ft::rvalue<unique_ptr>& r) throw();
	template <typename U, typename E>
	unique_ptr& operator=(ft::rvalue<unique_ptr<U, E> >& r) throw();
	unique_ptr& operator=(ft::nullptr_t /*unused*/) throw();

	pointer release() throw();
	template <typename U>
	void reset(U ptr,
	           typename ft::enable_if<_is_compatible_raw_pointer<U>::value,
	                                  _enabler>::type /*unused*/
	           = _enabler()) throw();
	void reset(ft::nullptr_t /*unused*/ = FT_NULLPTR) throw();
	void swap(unique_ptr& other) throw();

	/**
	 * @note Make sure the returned raw pointer is no longer in use after the
	 * unique_ptr goes out of scope.
	 */
	pointer get() const throw();
	// C++98 did not have reference collapsing yet.
	typename ft::add_lvalue_reference<Deleter>::type get_deleter() throw();
	typename ft::add_lvalue_reference<const Deleter>::type get_deleter() const
	    throw();
	bool operator_bool() const throw();

	T& operator[](std::size_t i) const;

private:
	FT_MOVABLE_BUT_NOT_COPYABLE(unique_ptr)

	// 4) b,c) (2)
	template <typename Pointer>
	unique_ptr(Pointer p,
	           typename ft::add_rvalue_reference<
	               typename ft::remove_reference<Deleter>::type>::type d,
	           typename ft::enable_if<
	               ft::is_lvalue_reference<Deleter>::value
	                   && _is_compatible_raw_pointer<Pointer>::value,
	               _enabler>::type = _enabler()) throw();

	pointer _ptr;
	Deleter _deleter;
};

/**
 * https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique
 */
template <typename T>
FT_REQUIRES(ft::is_unbounded_array<T>::value)
(unique_ptr<T>)make_unique(std::size_t size);

/**
 * https://en.cppreference.com/w/cpp/memory/unique_ptr/make_unique
 */
template <typename T>
FT_REQUIRES(ft::is_unbounded_array<T>::value)
(unique_ptr<T>)make_unique_for_overwrite(std::size_t size);

/* shared_ptr */

/**
 * @brief Basic implementation of `shared_ptr`
 *
 * Limitations:
 * - No `weak_ptr`.
 * - No `enable_shared_from_this`.
 * - Limited `make_shared` implementation.
 * - No support for custom allocators.
 * - Not thread-safe and no atomic operations.
 *
 * https://en.cppreference.com/w/cpp/memory/shared_ptr
 *
 * @note Don't construct or `reset` a `shared_ptr` with a raw pointer to an
 * object that is already managed by a `shared_ptr`. This will lead to double
 * deletions.
 */
template <typename T>
class shared_ptr : public ft::safe_bool<shared_ptr<T> >,
                   public _shared_ptr::shared_ptr_access<T> {
private:
	struct _enabler {};

public:
	typedef typename ft::remove_extent<T>::type element_type;

	// 1)
	shared_ptr() throw();
	// 2)
	// Original in `std::shared_ptr` is not `explicit`.
	// NOLINTNEXTLINE(google-explicit-constructor)
	shared_ptr(ft::nullptr_t /*unused*/) throw();
	// 3)
	template <typename Y>
	explicit shared_ptr(Y* ptr,
	                    typename ft::enable_if<
	                        _shared_ptr::is_compatible_raw_pointee<Y, T>::value,
	                        _enabler>::type /*unused*/
	                    = _enabler());
	// 4)
	template <typename Y, typename Deleter>
	shared_ptr(Y* ptr,
	           Deleter d,
	           typename ft::enable_if<
	               _shared_ptr::is_compatible_raw_pointee<Y, T>::value,
	               _enabler>::type /*unused*/
	           = _enabler());
	// 5)
	template <typename Deleter>
	shared_ptr(ft::nullptr_t ptr, Deleter d);
	// 8)
	/**
	 * @note Use of this constructor leads to a dangling pointer unless `ptr`
	 * remains valid at least until the ownership group of `r` is destroyed.
	 */
	template <typename Y>
	shared_ptr(const shared_ptr<Y>& r, element_type* ptr) throw();
	template <typename Y>
	shared_ptr(ft::rvalue<shared_ptr<Y> >& r, element_type* ptr) throw();
	// 9)
	shared_ptr(const shared_ptr& r) throw();
	// NOLINTBEGIN(google-explicit-constructor): Originals in `std::shared_ptr`
	// are not `explicit`.
	template <typename Y>
	shared_ptr(const shared_ptr<Y>& r,
	           typename ft::enable_if<
	               _shared_ptr::is_compatible_smart_pointer<Y, T>::value,
	               _enabler>::type /*unused*/
	           = _enabler()) throw();
	// 10)
	shared_ptr(ft::rvalue<shared_ptr>& r) throw();
	template <typename Y>
	shared_ptr(ft::rvalue<shared_ptr<Y> >& r,
	           typename ft::enable_if<
	               _shared_ptr::is_compatible_smart_pointer<Y, T>::value,
	               _enabler>::type /*unused*/
	           = _enabler()) throw();
#	if __cplusplus <= 201402L
	// 12)
	template <typename Y>
	shared_ptr(ft::rvalue<std::auto_ptr<Y> >& r,
	           typename ft::enable_if<ft::is_convertible<Y*, T*>::value,
	                                  _enabler>::type /*unused*/
	           = _enabler());
#	endif // __cplusplus <= 201402L
	// 13)
	template <typename Y, typename Deleter>
	shared_ptr(
	    ft::rvalue<unique_ptr<Y, Deleter> >& r,
	    typename ft::enable_if<
	        _shared_ptr::is_compatible_smart_pointer<Y, T>::value
	            && ft::is_convertible<typename unique_ptr<Y, Deleter>::pointer,
	                                  element_type*>::value,
	        _enabler>::type /*unused*/
	    = _enabler());
	// NOLINTEND(google-explicit-constructor)
	~shared_ptr();
	// Take by value to allow for more copy-elision.
	shared_ptr& operator=(shared_ptr r) throw();

	void reset() throw();
	template <typename Y>
	void reset(Y* ptr);
	template <typename Y, typename Deleter>
	void reset(Y* ptr, Deleter d);
	void swap(shared_ptr& r) throw();

	element_type* get() const throw();
	long use_count() const throw();
	bool operator_bool() const throw();
	template <typename Y>
	bool owner_before(const shared_ptr<Y>& other) const throw();
	template <typename Y>
	bool owner_equal(const shared_ptr<Y>& other) const throw();

private:
	FT_COPYABLE_AND_MOVABLE_SWAP(shared_ptr)

	template <typename>
	friend class shared_ptr;
	template <typename Deleter, typename U>
	friend Deleter* get_deleter(const shared_ptr<U>& p) throw();

	element_type* _ptr;
	_shared_ptr::control_block_base* _control;
};

/**
 * Limitations:
 * - Not a single allocation.
 * - No support for initial values for arrays.
 *
 * https://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared
 */
template <typename T>
FT_REQUIRES(!ft::is_array<T>::value)
(shared_ptr<T>)make_shared();
template <typename T, typename A0>
FT_REQUIRES(!ft::is_array<T>::value)
(shared_ptr<T>)make_shared(const A0& arg0);
template <typename T, typename A0, typename A1>
FT_REQUIRES(!ft::is_array<T>::value)
(shared_ptr<T>)make_shared(const A0& arg0, const A1& arg1);
template <typename T, typename A0, typename A1, typename A2>
FT_REQUIRES(!ft::is_array<T>::value)
(shared_ptr<T>)make_shared(const A0& arg0, const A1& arg1, const A2& arg2);
template <typename T, typename A0, typename A1, typename A2, typename A3>
FT_REQUIRES(!ft::is_array<T>::value)
(shared_ptr<T>)
    make_shared(const A0& arg0, const A1& arg1, const A2& arg2, const A3& arg3);
template <typename T,
          typename A0,
          typename A1,
          typename A2,
          typename A3,
          typename A4>
FT_REQUIRES(!ft::is_array<T>::value)
(shared_ptr<T>)make_shared(const A0& arg0,
                           const A1& arg1,
                           const A2& arg2,
                           const A3& arg3,
                           const A4& arg4);
template <typename T,
          typename A0,
          typename A1,
          typename A2,
          typename A3,
          typename A4,
          typename A5>
FT_REQUIRES(!ft::is_array<T>::value)
(shared_ptr<T>)make_shared(const A0& arg0,
                           const A1& arg1,
                           const A2& arg2,
                           const A3& arg3,
                           const A4& arg4,
                           const A5& arg5);
template <typename T,
          typename A0,
          typename A1,
          typename A2,
          typename A3,
          typename A4,
          typename A5,
          typename A6>
FT_REQUIRES(!ft::is_array<T>::value)
(shared_ptr<T>)make_shared(const A0& arg0,
                           const A1& arg1,
                           const A2& arg2,
                           const A3& arg3,
                           const A4& arg4,
                           const A5& arg5,
                           const A6& arg6);
template <typename T,
          typename A0,
          typename A1,
          typename A2,
          typename A3,
          typename A4,
          typename A5,
          typename A6,
          typename A7>
FT_REQUIRES(!ft::is_array<T>::value)
(shared_ptr<T>)make_shared(const A0& arg0,
                           const A1& arg1,
                           const A2& arg2,
                           const A3& arg3,
                           const A4& arg4,
                           const A5& arg5,
                           const A6& arg6,
                           const A7& arg7);
template <typename T,
          typename A0,
          typename A1,
          typename A2,
          typename A3,
          typename A4,
          typename A5,
          typename A6,
          typename A7,
          typename A8>
FT_REQUIRES(!ft::is_array<T>::value)
(shared_ptr<T>)make_shared(const A0& arg0,
                           const A1& arg1,
                           const A2& arg2,
                           const A3& arg3,
                           const A4& arg4,
                           const A5& arg5,
                           const A6& arg6,
                           const A7& arg7,
                           const A8& arg8);
template <typename T,
          typename A0,
          typename A1,
          typename A2,
          typename A3,
          typename A4,
          typename A5,
          typename A6,
          typename A7,
          typename A8,
          typename A9>
FT_REQUIRES(!ft::is_array<T>::value)
(shared_ptr<T>)make_shared(const A0& arg0,
                           const A1& arg1,
                           const A2& arg2,
                           const A3& arg3,
                           const A4& arg4,
                           const A5& arg5,
                           const A6& arg6,
                           const A7& arg7,
                           const A8& arg8,
                           const A9& arg9);
template <typename T>
FT_REQUIRES(ft::is_unbounded_array<T>::value)
(shared_ptr<T>)make_shared(std::size_t N);
template <typename T>
FT_REQUIRES(ft::is_bounded_array<T>::value)
(shared_ptr<T>)make_shared();

/**
 * https://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared
 */
template <typename T>
FT_REQUIRES(!ft::is_unbounded_array<T>::value)
(shared_ptr<T>)make_shared_for_overwrite();
template <typename T>
FT_REQUIRES(ft::is_unbounded_array<T>::value)
(shared_ptr<T>)make_shared_for_overwrite(std::size_t N);

/**
 * https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast
 */
template <typename T, typename U>
shared_ptr<T> static_pointer_cast(const shared_ptr<U>& r) throw();
template <typename T, typename U>
shared_ptr<T> static_pointer_cast(ft::rvalue<shared_ptr<U> >& r) throw();
template <typename T, typename U>
shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& r) throw();
template <typename T, typename U>
shared_ptr<T> dynamic_pointer_cast(ft::rvalue<shared_ptr<U> >& r) throw();
template <typename T, typename U>
shared_ptr<T> const_pointer_cast(const shared_ptr<U>& r) throw();
template <typename T, typename U>
shared_ptr<T> const_pointer_cast(ft::rvalue<shared_ptr<U> >& r) throw();
template <typename T, typename U>
shared_ptr<T> reinterpret_pointer_cast(const shared_ptr<U>& r) throw();
template <typename T, typename U>
shared_ptr<T> reinterpret_pointer_cast(ft::rvalue<shared_ptr<U> >& r) throw();

/**
 * https://en.cppreference.com/w/cpp/memory/shared_ptr/get_deleter
 */
template <typename Deleter, typename T>
Deleter* get_deleter(const shared_ptr<T>& p) throw();

template <typename T, typename U>
bool operator==(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) throw();
template <typename T, typename U>
bool operator!=(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) throw();
template <typename T, typename U>
bool operator<(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) throw();
template <typename T, typename U>
bool operator>(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) throw();
template <typename T, typename U>
bool operator<=(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) throw();
template <typename T, typename U>
bool operator>=(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) throw();
template <typename T>
bool operator==(const shared_ptr<T>& lhs, ft::nullptr_t /*unused*/) throw();
template <typename T>
bool operator==(ft::nullptr_t /*unused*/, const shared_ptr<T>& rhs) throw();
template <typename T>
bool operator!=(const shared_ptr<T>& lhs, ft::nullptr_t /*unused*/) throw();
template <typename T>
bool operator!=(ft::nullptr_t /*unused*/, const shared_ptr<T>& rhs) throw();
template <typename T>
bool operator<(const shared_ptr<T>& lhs, ft::nullptr_t /*unused*/) throw();
template <typename T>
bool operator<(ft::nullptr_t /*unused*/, const shared_ptr<T>& rhs) throw();
template <typename T>
bool operator>(const shared_ptr<T>& lhs, ft::nullptr_t /*unused*/) throw();
template <typename T>
bool operator>(ft::nullptr_t /*unused*/, const shared_ptr<T>& rhs) throw();
template <typename T>
bool operator<=(const shared_ptr<T>& lhs, ft::nullptr_t /*unused*/) throw();
template <typename T>
bool operator<=(ft::nullptr_t /*unused*/, const shared_ptr<T>& rhs) throw();
template <typename T>
bool operator>=(const shared_ptr<T>& lhs, ft::nullptr_t /*unused*/) throw();
template <typename T>
bool operator>=(ft::nullptr_t /*unused*/, const shared_ptr<T>& rhs) throw();

template <typename T, typename U, typename V>
std::basic_ostream<U, V>& operator<<(std::basic_ostream<U, V>& os,
                                     const shared_ptr<T>& ptr);

template <typename T>
void swap(shared_ptr<T>& lhs, shared_ptr<T>& rhs) throw();

/* owner comparison */

/**
 * https://en.cppreference.com/w/cpp/memory/owner_less
 */
template <typename T = void>
struct owner_less;

template <typename T>
struct owner_less<shared_ptr<T> > {
	bool operator()(const shared_ptr<T>& lhs, const shared_ptr<T>& rhs) const
	    throw();
};

/**
 * https://en.cppreference.com/w/cpp/memory/owner_less_void
 */
template <>
struct owner_less<void> {
	typedef void is_transparent;

	template <typename T, typename U>
	bool operator()(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) const
	    throw();
};

/**
 * https://en.cppreference.com/w/cpp/memory/owner_equal
 */
struct owner_equal {
	typedef void is_transparent;

	template <typename T, typename U>
	bool operator()(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) const
	    throw();
};

/* addressof */

// NOLINTBEGIN(readability-redundant-declaration): Included from
// shared_ptr_detail.hpp.

/**
 * https://en.cppreference.com/w/cpp/memory/addressof
 */
template <typename T>
T* addressof(T& arg) throw();
// NOLINTEND(readability-redundant-declaration)

} // namespace ft

#	include "libftpp/memory/addressof.tpp"        // IWYU pragma: export
#	include "libftpp/memory/default_delete.tpp"   // IWYU pragma: export
#	include "libftpp/memory/owner_comparison.tpp" // IWYU pragma: export
#	include "libftpp/memory/shared_ptr.tpp"       // IWYU pragma: export
#	include "libftpp/memory/unique_ptr.tpp"       // IWYU pragma: export
#	include "libftpp/memory/unique_ptr_array.tpp" // IWYU pragma: export

#endif // LIBFTPP_MEMORY_HPP
