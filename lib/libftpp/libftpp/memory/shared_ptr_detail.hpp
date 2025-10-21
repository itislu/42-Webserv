#pragma once
#ifndef LIBFTPP_MEMORY_SHARED_PTR_DETAIL_HPP
#	define LIBFTPP_MEMORY_SHARED_PTR_DETAIL_HPP

#	include "libftpp/type_traits.hpp"
#	include <cstddef>
#	include <typeinfo>

namespace ft {

// Forward declaration.
template <typename T>
class shared_ptr;

namespace _shared_ptr {

/**
 * If T is an array type U[N], checks whether Y(*)[N] is a valid type and
 * convertible to T*. If T is an array type U[], checks whether Y(*)[] is a
 * valid type convertible to T*. Otherwise, checks whether Y* is convertible to
 * T*.
 */
template <typename Y, typename T, typename = void>
struct is_compatible_raw_pointee;

/**
 * A pointer type Y* is said to be compatible with a pointer type T* if either
 * Y* is convertible to T* or Y is the array type U[N] and T is U cv [] (where
 * cv is some set of cv-qualifiers).
 */
template <typename Y, typename T>
struct is_compatible_smart_pointer;

/**
 * Correctly deletes `ptr` with `delete` or `delete[]`.
 */
template <typename T, typename Yp>
void delete_ptr(Yp ptr) throw();

/* Conditionally inherited access operators */

/**
 * Helper class to get stored pointer from `shared_ptr`.
 */
template <typename T>
class shared_ptr_get {
public:
	typedef typename ft::remove_extent<T>::type element_type;

protected:
	element_type* get() const throw();
};

/**
 * Non-array, non-void.
 */
template <typename T, typename = void>
class shared_ptr_access : public shared_ptr_get<T> {
public:
	T& operator*() const throw();
	T* operator->() const throw();
};

/**
 * Array specialization.
 */
template <typename T>
class shared_ptr_access<T, typename ft::enable_if<ft::is_array<T>::value>::type>
    : public shared_ptr_get<T> {
public:
	typename shared_ptr_get<T>::element_type&
	operator[](std::ptrdiff_t idx) const;
};

/**
 * Cv void specialization.
 */
template <typename T>
class shared_ptr_access<T, typename ft::enable_if<ft::is_void<T>::value>::type>
    : public shared_ptr_get<T> {
public:
	T* operator->() const throw();
};

/* Control blocks */

class control_block_base {
public:
	virtual ~control_block_base();

	virtual void dispose() throw() = 0;
	virtual void* get_deleter(const std::type_info& t) throw() = 0;
	long use_count() const throw();
	void add_shared() throw();
	bool release() throw();

protected:
	control_block_base() throw();

private:
	control_block_base(const control_block_base&);
	control_block_base& operator=(const control_block_base&);

	long _use_count;
};

template <typename Yp, typename T>
class control_block_pointer : public control_block_base {
public:
	explicit control_block_pointer(Yp ptr) throw();

	virtual void dispose() throw();
	virtual void* get_deleter(const std::type_info& t) throw();

private:
	Yp _ptr;
};

template <typename Yp, typename Deleter>
class control_block_pointer_deleter : public control_block_base {
public:
	control_block_pointer_deleter(Yp ptr, Deleter d) throw();

	virtual void dispose() throw();
	virtual void* get_deleter(const std::type_info& t) throw();

private:
	Yp _ptr;
	Deleter _deleter;
};

} // namespace _shared_ptr

} // namespace ft

#	include "shared_ptr_access.tpp"          // IWYU pragma: export
#	include "shared_ptr_control_derived.tpp" // IWYU pragma: export
#	include "shared_ptr_detail.tpp"          // IWYU pragma: export

#endif // LIBFTPP_MEMORY_SHARED_PTR_DETAIL_HPP
