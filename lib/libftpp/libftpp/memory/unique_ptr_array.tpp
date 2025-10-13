#pragma once
#ifndef LIBFTPP_MEMORY_UNIQUE_PTR_ARRAY_TPP
#	define LIBFTPP_MEMORY_UNIQUE_PTR_ARRAY_TPP

#	include "libftpp/memory.hpp"
#	include "unique_ptr_detail.hpp"
#	include "libftpp/assert.hpp"
#	include "libftpp/movable.hpp"
#	include "libftpp/type_traits.hpp"
#	include "libftpp/utility.hpp"
#	include <algorithm>
#	include <cassert>
#	include <cstddef>

namespace ft {

template <typename T, typename Deleter /*= default_delete<T> */>
template <typename U>
struct unique_ptr<T[], Deleter>::_is_compatible_raw_pointer
    : ft::bool_constant<
          ft::is_same<U, pointer>::value
          || (ft::is_same<pointer, element_type*>::value
              && ft::is_convertible<typename ft::remove_pointer<U>::type (*)[],
                                    element_type (*)[]>::value)> {};

template <typename T, typename Deleter /*= default_delete<T> */>
template <typename U, typename E>
struct unique_ptr<T[], Deleter>::_is_compatible_unique_ptr
    : ft::bool_constant<
          ft::is_array<U>::value && ft::is_same<pointer, element_type*>::value
          && ft::is_same<typename unique_ptr<U, E>::pointer,
                         typename unique_ptr<U, E>::element_type*>::value
          && ft::is_convertible<typename unique_ptr<U, E>::element_type (*)[],
                                element_type (*)[]>::value> {};

// 1)
template <typename T, typename Deleter /*= default_delete<T> */>
unique_ptr<T[], Deleter>::unique_ptr() throw()
    : _p(),
      _d()
{
	FT_STATIC_ASSERT(!ft::is_pointer<Deleter>::value);
}

// 2)
template <typename T, typename Deleter /*= default_delete<T> */>
template <typename U>
unique_ptr<T[], Deleter>::unique_ptr(
    U p,
    typename ft::enable_if<!ft::is_pointer<Deleter>::value
                               && _is_compatible_raw_pointer<U>::value,
                           _enabler>::type /*unused = _enabler()*/) throw()
    : _p(p),
      _d()
{}

// 3) a,b,c) (1)
template <typename T, typename Deleter /*= default_delete<T> */>
template <typename U>
unique_ptr<T[], Deleter>::unique_ptr(
    U p,
    typename ft::conditional<
        !ft::is_reference<Deleter>::value,
        typename ft::add_lvalue_reference<const Deleter>::type,
        Deleter>::type d,
    typename ft::enable_if<_is_compatible_raw_pointer<U>::value,
                           _enabler>::type /*unused = _enabler()*/) throw()
    : _p(p),
      _d(d)
{}

// 4) a) (2)
template <typename T, typename Deleter /*= default_delete<T> */>
template <typename U>
unique_ptr<T[], Deleter>::unique_ptr(
    U p,
    ft::rvalue<Deleter>& d,
    typename ft::enable_if<!ft::is_reference<Deleter>::value
                               && _is_compatible_raw_pointer<U>::value,
                           _enabler>::type /*unused = _enabler()*/) throw()
    : _p(p),
      _d(ft::move(d))
{}

// 5)
template <typename T, typename Deleter /*= default_delete<T> */>
unique_ptr<T[], Deleter>::unique_ptr(ft::rvalue<unique_ptr>& u) throw()
    : _p(u.release()),
      _d(ft::move(u._d))
{}

// 6)
template <typename T, typename Deleter /*= default_delete<T> */>
template <typename U, typename E>
unique_ptr<T[], Deleter>::unique_ptr(
    ft::rvalue<unique_ptr<U, E> >& u,
    typename ft::enable_if<
        _is_compatible_unique_ptr<U, E>::value
            && _unique_ptr::is_compatible_deleter<E, Deleter>::value,
        _enabler>::type /*unused = _enabler()*/) throw()
    : _p(u.release()),
      _d(ft::is_reference<E>::value ? u.get_deleter()
                                    : ft::move(u.get_deleter()))
{}

template <typename T, typename Deleter /*= default_delete<T> */>
unique_ptr<T[], Deleter>::~unique_ptr()
{
	reset();
}

template <typename T, typename Deleter /*= default_delete<T> */>
unique_ptr<T[], Deleter>&
unique_ptr<T[], Deleter>::operator=(ft::rvalue<unique_ptr>& r) throw()
{
	reset(r.release());
	_d = ft::move(r._d);
	return *this;
}

template <typename T, typename Deleter /*= default_delete<T> */>
template <typename U, typename E>
unique_ptr<T[], Deleter>&
unique_ptr<T[], Deleter>::operator=(ft::rvalue<unique_ptr<U, E> >& r) throw()
{
	FT_STATIC_ASSERT((_is_compatible_unique_ptr<U, E>::value));

	reset(r.release());
	_d = ft::move(r.get_deleter());
	return *this;
}

template <typename T, typename Deleter /*= default_delete<T> */>
typename unique_ptr<T[], Deleter>::pointer
unique_ptr<T[], Deleter>::release() throw()
{
	pointer p = _p;
	_p = pointer();
	return p;
}

template <typename T, typename Deleter /*= default_delete<T> */>
template <typename U>
void unique_ptr<T[], Deleter>::reset(
    U ptr,
    typename ft::enable_if<_is_compatible_raw_pointer<U>::value,
                           _enabler>::type /*unused = _enabler()*/) throw()
{
	assert(ptr == pointer() || ptr != _p);

	pointer old_ptr = _p;
	_p = ptr;
	if (old_ptr != pointer()) {
		_d(old_ptr);
	}
}

template <typename T, typename Deleter /*= default_delete<T> */>
void unique_ptr<T[], Deleter>::reset() throw()
{
	reset(pointer());
}

template <typename T, typename Deleter /*= default_delete<T> */>
void unique_ptr<T[], Deleter>::swap(unique_ptr& other) throw()
{
	using std::swap;
	swap(_p, other._p);
	swap(_d, other._d);
}

template <typename T, typename Deleter /*= default_delete<T> */>
typename unique_ptr<T[], Deleter>::pointer unique_ptr<T[], Deleter>::get() const
    throw()
{
	return _p;
}

template <typename T, typename Deleter /*= default_delete<T> */>
typename ft::add_lvalue_reference<Deleter>::type
unique_ptr<T[], Deleter>::get_deleter() throw()
{
	return _d;
}

template <typename T, typename Deleter /*= default_delete<T> */>
typename ft::add_lvalue_reference<const Deleter>::type
unique_ptr<T[], Deleter>::get_deleter() const throw()
{
	return _d;
}

template <typename T, typename Deleter /*= default_delete<T> */>
bool unique_ptr<T[], Deleter>::boolean_test() const throw()
{
	return _p != pointer();
}

template <typename T, typename Deleter /*= default_delete<T> */>
T& unique_ptr<T[], Deleter>::operator[](std::size_t i) const
{
	assert(_p != pointer());
	return _p[i]; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
}

template <typename T>
FT_REQUIRES(ft::is_unbounded_array<T>::value)
(unique_ptr<T>)make_unique(std::size_t size)
{
	return unique_ptr<T>(new typename ft::remove_extent<T>::type[size]());
}

template <typename T>
FT_REQUIRES(ft::is_unbounded_array<T>::value)
(unique_ptr<T>)make_unique_for_overwrite(std::size_t size)
{
	return unique_ptr<T>(new typename ft::remove_extent<T>::type[size]);
}

} // namespace ft

#endif // LIBFTPP_MEMORY_UNIQUE_PTR_ARRAY_TPP
