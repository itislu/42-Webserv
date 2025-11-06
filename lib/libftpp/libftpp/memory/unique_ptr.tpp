// IWYU pragma: private; include "libftpp/memory.hpp"
#pragma once
#ifndef LIBFTPP_MEMORY_UNIQUE_PTR_TPP
#	define LIBFTPP_MEMORY_UNIQUE_PTR_TPP

#	include "libftpp/memory.hpp"
#	include "unique_ptr_detail.hpp"
#	include "libftpp/assert.hpp"
#	include "libftpp/functional.hpp"
#	include "libftpp/movable.hpp"
#	include "libftpp/type_traits.hpp"
#	include "libftpp/utility.hpp"
#	include <algorithm>
#	include <cassert>
#	include <memory>
#	include <ostream>

namespace ft {

template <typename T, typename Deleter /*= default_delete<T> */>
template <typename U, typename E>
struct unique_ptr<T, Deleter>::_is_compatible_unique_ptr
    : ft::bool_constant<
          !ft::is_array<U>::value
          && ft::is_convertible<typename unique_ptr<U, E>::pointer,
                                pointer>::value> {};

// 1)
template <typename T, typename Deleter /*= default_delete<T> */>
unique_ptr<T, Deleter>::unique_ptr() throw()
    : _ptr(),
      _deleter()
{
	FT_STATIC_ASSERT(!ft::is_pointer<Deleter>::value);
}

template <typename T, typename Deleter /*= default_delete<T> */>
unique_ptr<T, Deleter>::unique_ptr(ft::nullptr_t /*unused*/) throw()
    : _ptr(),
      _deleter()
{
	FT_STATIC_ASSERT(!ft::is_pointer<Deleter>::value);
}

// 2)
template <typename T, typename Deleter /*= default_delete<T> */>
template <typename Pointer>
unique_ptr<T, Deleter>::unique_ptr(
    Pointer p,
    typename ft::enable_if<ft::is_convertible<Pointer, pointer>::value
                               && !ft::is_pointer<Deleter>::value,
                           _enabler>::type /*unused = _enabler()*/) throw()
    : _ptr(p),
      _deleter()
{}

// 3) a,b,c) (1)
template <typename T, typename Deleter /*= default_delete<T> */>
unique_ptr<T, Deleter>::unique_ptr(
    pointer p,
    typename ft::conditional<
        !ft::is_reference<Deleter>::value,
        typename ft::add_lvalue_reference<const Deleter>::type,
        Deleter>::type d) throw()
    : _ptr(p),
      _deleter(d)
{}

// 4) a) (2)
template <typename T, typename Deleter /*= default_delete<T> */>
template <typename Pointer>
unique_ptr<T, Deleter>::unique_ptr(
    Pointer p,
    ft::rvalue<Deleter>& d,
    typename ft::enable_if<ft::is_convertible<Pointer, pointer>::value
                               && !ft::is_reference<Deleter>::value,
                           _enabler>::type /*unused = _enabler()*/) throw()
    : _ptr(p),
      _deleter(ft::move(d))
{}

// 5)
template <typename T, typename Deleter /*= default_delete<T> */>
unique_ptr<T, Deleter>::unique_ptr(ft::rvalue<unique_ptr>& u) throw()
    : _ptr(u.release()),
      _deleter(ft::move(u._deleter))
{}

// 6)
template <typename T, typename Deleter /*= default_delete<T> */>
template <typename U, typename E>
unique_ptr<T, Deleter>::unique_ptr(
    ft::rvalue<unique_ptr<U, E> >& u,
    typename ft::enable_if<
        _is_compatible_unique_ptr<U, E>::value
            && _unique_ptr::is_compatible_deleter<E, Deleter>::value,
        _enabler>::type /*unused = _enabler()*/) throw()
    : _ptr(u.release()),
      _deleter(ft::is_reference<E>::value ? u.get_deleter()
                                          : ft::move(u.get_deleter()))
{}

#	if LIBFTPP_SUPPORT_AUTO_PTR
// 8)
template <typename T, typename Deleter /*= default_delete<T> */>
template <typename U>
unique_ptr<T, Deleter>::unique_ptr(
    ft::rvalue<std::auto_ptr<U> >& u,
    typename ft::enable_if<
        ft::is_convertible<U*, T*>::value
            && ft::is_same<Deleter, default_delete<T> >::value,
        _enabler>::type /*unused = _enabler()*/) throw()
    : _ptr(u.release()),
      _deleter()
{}
#	endif // LIBFTPP_SUPPORT_AUTO_PTR

template <typename T, typename Deleter /*= default_delete<T> */>
unique_ptr<T, Deleter>::~unique_ptr()
{
	reset();
}

template <typename T, typename Deleter /*= default_delete<T> */>
unique_ptr<T, Deleter>&
unique_ptr<T, Deleter>::operator=(ft::rvalue<unique_ptr>& r) throw()
{
	reset(r.release());
	_deleter = ft::move(r._deleter);
	return *this;
}

template <typename T, typename Deleter /*= default_delete<T> */>
template <typename U, typename E>
unique_ptr<T, Deleter>&
unique_ptr<T, Deleter>::operator=(ft::rvalue<unique_ptr<U, E> >& r) throw()
{
	FT_STATIC_ASSERT((_is_compatible_unique_ptr<U, E>::value));

	reset(r.release());
	_deleter = ft::move(r.get_deleter());
	return *this;
}

template <typename T, typename Deleter /*= default_delete<T> */>
unique_ptr<T, Deleter>&
unique_ptr<T, Deleter>::operator=(ft::nullptr_t /*unused*/) throw()
{
	reset();
	return *this;
}

template <typename T, typename Deleter /*= default_delete<T> */>
typename unique_ptr<T, Deleter>::pointer
unique_ptr<T, Deleter>::release() throw()
{
	pointer p = _ptr;
	_ptr = pointer();
	return p;
}

template <typename T, typename Deleter /*= default_delete<T> */>
void unique_ptr<T, Deleter>::reset(pointer ptr /*= pointer()*/) throw()
{
	assert(ptr == pointer() || ptr != _ptr);

	pointer old_ptr = _ptr;
	_ptr = ptr;
	if (old_ptr != pointer()) {
		_deleter(old_ptr);
	}
}

template <typename T, typename Deleter /*= default_delete<T> */>
void unique_ptr<T, Deleter>::swap(unique_ptr& other) throw()
{
	using std::swap;
	swap(_ptr, other._ptr);
	swap(_deleter, other._deleter);
}

template <typename T, typename Deleter /*= default_delete<T> */>
typename unique_ptr<T, Deleter>::pointer unique_ptr<T, Deleter>::get() const
    throw()
{
	return _ptr;
}

template <typename T, typename Deleter /*= default_delete<T> */>
typename ft::add_lvalue_reference<Deleter>::type
unique_ptr<T, Deleter>::get_deleter() throw()
{
	return _deleter;
}

template <typename T, typename Deleter /*= default_delete<T> */>
typename ft::add_lvalue_reference<const Deleter>::type
unique_ptr<T, Deleter>::get_deleter() const throw()
{
	return _deleter;
}

template <typename T, typename Deleter /*= default_delete<T> */>
bool unique_ptr<T, Deleter>::operator_bool() const throw()
{
	return _ptr != pointer();
}

template <typename T, typename Deleter /*= default_delete<T> */>
typename ft::add_lvalue_reference<T>::type
unique_ptr<T, Deleter>::operator*() const
{
	assert(_ptr != pointer());
	return *_ptr;
}

template <typename T, typename Deleter /*= default_delete<T> */>
typename unique_ptr<T, Deleter>::pointer
unique_ptr<T, Deleter>::operator->() const throw()
{
	assert(_ptr != pointer());
	return _ptr;
}

template <typename T>
FT_REQUIRES(!ft::is_array<T>::value)
(unique_ptr<T>)make_unique() { return unique_ptr<T>(new T()); }

template <typename T, typename A0>
FT_REQUIRES(!ft::is_array<T>::value)
(unique_ptr<T>)make_unique(const A0& arg0)
{
	return unique_ptr<T>(new T(arg0));
}

template <typename T, typename A0, typename A1>
FT_REQUIRES(!ft::is_array<T>::value)
(unique_ptr<T>)make_unique(const A0& arg0, const A1& arg1)
{
	return unique_ptr<T>(new T(arg0, arg1));
}

template <typename T, typename A0, typename A1, typename A2>
FT_REQUIRES(!ft::is_array<T>::value)
(unique_ptr<T>)make_unique(const A0& arg0, const A1& arg1, const A2& arg2)
{
	return unique_ptr<T>(new T(arg0, arg1, arg2));
}

template <typename T, typename A0, typename A1, typename A2, typename A3>
FT_REQUIRES(!ft::is_array<T>::value)
(unique_ptr<T>)
    make_unique(const A0& arg0, const A1& arg1, const A2& arg2, const A3& arg3)
{
	return unique_ptr<T>(new T(arg0, arg1, arg2, arg3));
}

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
                           const A4& arg4)
{
	return unique_ptr<T>(new T(arg0, arg1, arg2, arg3, arg4));
}

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
                           const A5& arg5)
{
	return unique_ptr<T>(new T(arg0, arg1, arg2, arg3, arg4, arg5));
}

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
                           const A6& arg6)
{
	return unique_ptr<T>(new T(arg0, arg1, arg2, arg3, arg4, arg5, arg6));
}

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
                           const A7& arg7)
{
	return unique_ptr<T>(new T(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7));
}

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
                           const A8& arg8)
{
	return unique_ptr<T>(
	    new T(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8));
}

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
                           const A9& arg9)
{
	return unique_ptr<T>(
	    new T(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9));
}

template <typename T>
FT_REQUIRES(!ft::is_array<T>::value)
(unique_ptr<T>)make_unique_for_overwrite() { return unique_ptr<T>(new T); }

template <typename T1, typename D1, typename T2, typename D2>
bool operator==(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y)
{
	return x.get() == y.get();
}

template <typename T1, typename D1, typename T2, typename D2>
bool operator!=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y)
{
	return !(x == y);
}

template <typename T1, typename D1, typename T2, typename D2>
bool operator<(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y)
{
	return x.get() < y.get();
}

template <typename T1, typename D1, typename T2, typename D2>
bool operator<=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y)
{
	return !(y < x);
}

template <typename T1, typename D1, typename T2, typename D2>
bool operator>(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y)
{
	return y < x;
}

template <typename T1, typename D1, typename T2, typename D2>
bool operator>=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y)
{
	return !(x < y);
}

template <typename T, typename D>
bool operator==(const unique_ptr<T, D>& x, ft::nullptr_t /*unused*/) throw()
{
	return !x;
}

template <typename T, typename D>
bool operator==(ft::nullptr_t /*unused*/, const unique_ptr<T, D>& x) throw()
{
	return !x;
}

template <typename T, typename D>
bool operator!=(const unique_ptr<T, D>& x, ft::nullptr_t /*unused*/) throw()
{
	return static_cast<bool>(x);
}

template <typename T, typename D>
bool operator!=(ft::nullptr_t /*unused*/, const unique_ptr<T, D>& x) throw()
{
	return static_cast<bool>(x);
}

template <typename T, typename D>
bool operator<(const unique_ptr<T, D>& x, ft::nullptr_t /*unused*/)
{
	return ft::less<typename unique_ptr<T, D>::pointer>()(x.get(), FT_NULLPTR);
}

template <typename T, typename D>
bool operator<(ft::nullptr_t /*unused*/, const unique_ptr<T, D>& y)
{
	return ft::less<typename unique_ptr<T, D>::pointer>()(FT_NULLPTR, y.get());
}

template <typename T, typename D>
bool operator<=(const unique_ptr<T, D>& x, ft::nullptr_t /*unused*/)
{
	return !(FT_NULLPTR < x);
}

template <typename T, typename D>
bool operator<=(ft::nullptr_t /*unused*/, const unique_ptr<T, D>& y)
{
	return !(y < FT_NULLPTR);
}

template <typename T, typename D>
bool operator>(const unique_ptr<T, D>& x, ft::nullptr_t /*unused*/)
{
	return FT_NULLPTR < x;
}

template <typename T, typename D>
bool operator>(ft::nullptr_t /*unused*/, const unique_ptr<T, D>& y)
{
	return y < FT_NULLPTR;
}

template <typename T, typename D>
bool operator>=(const unique_ptr<T, D>& x, ft::nullptr_t /*unused*/)
{
	return !(x < FT_NULLPTR);
}

template <typename T, typename D>
bool operator>=(ft::nullptr_t /*unused*/, const unique_ptr<T, D>& y)
{
	return !(FT_NULLPTR < y);
}

template <typename CharT, typename Traits, typename Y, typename D>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const unique_ptr<Y, D>& p)
{
	return os << p.get();
}

template <typename T, typename D>
void swap(unique_ptr<T, D>& lhs, unique_ptr<T, D>& rhs) throw()
{
	lhs.swap(rhs);
}

} // namespace ft

#endif // LIBFTPP_MEMORY_UNIQUE_PTR_TPP
