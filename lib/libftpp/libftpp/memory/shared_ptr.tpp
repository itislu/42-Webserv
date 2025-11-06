// IWYU pragma: private; include "libftpp/memory.hpp"
#pragma once
#ifndef LIBFTPP_MEMORY_SHARED_PTR_TPP
#	define LIBFTPP_MEMORY_SHARED_PTR_TPP

#	include "libftpp/memory.hpp"
#	include "shared_ptr_detail.hpp"
#	include "libftpp/assert.hpp"
#	include "libftpp/functional.hpp"
#	include "libftpp/movable.hpp"
#	include "libftpp/safe_bool.hpp"
#	include "libftpp/type_traits.hpp"
#	include "libftpp/utility.hpp"
#	include <algorithm>
#	include <cassert>
#	include <cstddef>
#	include <memory>
#	include <ostream>
#	include <typeinfo> // IWYU pragma: keep: typeid

namespace ft {

// 1)
template <typename T>
shared_ptr<T>::shared_ptr() throw()
    : _ptr(FT_NULLPTR),
      _control(FT_NULLPTR)
{}

// 2)
template <typename T>
shared_ptr<T>::shared_ptr(ft::nullptr_t /*unused*/) throw()
    : _ptr(FT_NULLPTR),
      _control(FT_NULLPTR)
{}

// 3)
template <typename T>
template <typename Y>
shared_ptr<T>::shared_ptr(
    Y* ptr,
    typename ft::enable_if<_shared_ptr::is_compatible_raw_pointee<Y, T>::value,
                           _enabler>::type /*unused = _enabler()*/)
try
    : _ptr(ptr),
      _control(new _shared_ptr::control_block_pointer<Y*, T>(ptr)) {
	FT_STATIC_ASSERT(ft::is_complete<Y>::value); // Y must be a complete type.
}
catch (...) {
	_shared_ptr::delete_ptr<T>(ptr);
	throw;
}

// 4)
template <typename T>
template <typename Y, typename Deleter>
shared_ptr<T>::shared_ptr(
    Y* ptr,
    Deleter d,
    typename ft::enable_if<_shared_ptr::is_compatible_raw_pointee<Y, T>::value,
                           _enabler>::type /*unused = _enabler()*/)
try
    : _ptr(ptr),
      _control(new _shared_ptr::control_block_pointer_deleter<Y*, Deleter>(
          ptr,
          ft::move(d))) {
}
catch (...) {
	d(ptr);
	throw;
}

// 5
template <typename T>
template <typename Deleter>
shared_ptr<T>::shared_ptr(ft::nullptr_t ptr, Deleter d)
try
    : _ptr(ptr),
      _control(new _shared_ptr::control_block_pointer_deleter<ft::nullptr_t,
                                                              Deleter>(
          ptr,
          ft::move(d))) {
}
catch (...) {
	d(ptr);
	throw;
}

// 8)
template <typename T>
template <typename Y>
shared_ptr<T>::shared_ptr(const shared_ptr<Y>& r, element_type* ptr) throw()
    : _ptr(ptr),
      _control(r._control)
{
	if (_control != FT_NULLPTR) {
		_control->add_shared();
	}
}

template <typename T>
template <typename Y>
shared_ptr<T>::shared_ptr(ft::rvalue<shared_ptr<Y> >& r,
                          element_type* ptr) throw()
    : _ptr(ptr),
      _control(r._control)
{
	r._ptr = FT_NULLPTR;
	r._control = FT_NULLPTR;
}

// 9)
template <typename T>
shared_ptr<T>::shared_ptr(const shared_ptr& r) throw()
    : ft::safe_bool<shared_ptr>(),
      _ptr(r._ptr),
      _control(r._control)
{
	if (_control != FT_NULLPTR) {
		_control->add_shared();
	}
}

template <typename T>
template <typename Y>
shared_ptr<T>::shared_ptr(
    const shared_ptr<Y>& r,
    typename ft::enable_if<
        _shared_ptr::is_compatible_smart_pointer<Y, T>::value,
        _enabler>::type /*unused = _enabler()*/) throw()
    : _ptr(r._ptr),
      _control(r._control)
{
	if (_control != FT_NULLPTR) {
		_control->add_shared();
	}
}

// 10)
template <typename T>
shared_ptr<T>::shared_ptr(ft::rvalue<shared_ptr>& r) throw()
    : _ptr(r._ptr),
      _control(r._control)
{
	r._ptr = FT_NULLPTR;
	r._control = FT_NULLPTR;
}

template <typename T>
template <typename Y>
shared_ptr<T>::shared_ptr(
    ft::rvalue<shared_ptr<Y> >& r,
    typename ft::enable_if<
        _shared_ptr::is_compatible_smart_pointer<Y, T>::value,
        _enabler>::type /*unused = _enabler()*/) throw()
    : _ptr(r._ptr),
      _control(r._control)
{
	r._ptr = FT_NULLPTR;
	r._control = FT_NULLPTR;
}

#	if LIBFTPP_SUPPORT_AUTO_PTR
// 12)
template <typename T>
template <typename Y>
shared_ptr<T>::shared_ptr(
    ft::rvalue<std::auto_ptr<Y> >& r,
    typename ft::enable_if<ft::is_convertible<Y*, T*>::value,
                           _enabler>::type /*unused = _enabler()*/)
    : _ptr(r.get()),
      _control(r.get() != FT_NULLPTR
                   ? new _shared_ptr::control_block_pointer<Y*, T>(r.get())
                   : FT_NULLPTR)
{
	r.release();
}
#	endif // LIBFTPP_SUPPORT_AUTO_PTR

// 13)
template <typename T>
template <typename Y, typename Deleter>
shared_ptr<T>::shared_ptr(
    ft::rvalue<unique_ptr<Y, Deleter> >& r,
    typename ft::enable_if<
        _shared_ptr::is_compatible_smart_pointer<Y, T>::value
            && ft::is_convertible<typename unique_ptr<Y, Deleter>::pointer,
                                  element_type*>::value,
        _enabler>::type /*unused = _enabler()*/)
    : _ptr(r.get()),
      _control(r.get() != FT_NULLPTR
                   ? new _shared_ptr::control_block_pointer_deleter<
                         typename unique_ptr<Y, Deleter>::pointer,
                         Deleter>(r.get(), ft::move(r.get_deleter()))
                   : FT_NULLPTR)
{
	r.release();
}

template <typename T>
shared_ptr<T>::~shared_ptr()
{
	if (_control != FT_NULLPTR) {
		if (_control->release()) {
			delete _control;
		}
	}
}

template <typename T>
shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr r) throw()
{
	swap(r);
	return *this;
}

template <typename T>
void shared_ptr<T>::reset() throw()
{
	shared_ptr().swap(*this);
}

template <typename T>
template <typename Y>
void shared_ptr<T>::reset(Y* ptr)
{
	assert(ptr == FT_NULLPTR || ptr != _ptr);
	shared_ptr(ptr).swap(*this);
}

template <typename T>
template <typename Y, typename Deleter>
void shared_ptr<T>::reset(Y* ptr, Deleter d)
{
	assert(ptr == FT_NULLPTR || ptr != _ptr);
	shared_ptr(ptr, d).swap(*this);
}

template <typename T>
void shared_ptr<T>::swap(shared_ptr& r) throw()
{
	std::swap(_ptr, r._ptr);
	std::swap(_control, r._control);
}

template <typename T>
typename shared_ptr<T>::element_type* shared_ptr<T>::get() const throw()
{
	return _ptr;
}

template <typename T>
long shared_ptr<T>::use_count() const throw()
{
	return _control != FT_NULLPTR ? _control->use_count() : 0;
}

template <typename T>
bool shared_ptr<T>::operator_bool() const throw()
{
	return _ptr != FT_NULLPTR;
}

template <typename T>
template <typename Y>
bool shared_ptr<T>::owner_before(const shared_ptr<Y>& other) const throw()
{
	return _control < other._control;
}

template <typename T>
template <typename Y>
bool shared_ptr<T>::owner_equal(const shared_ptr<Y>& other) const throw()
{
	return _control == other._control;
}

template <typename T>
FT_REQUIRES(!ft::is_array<T>::value)
(shared_ptr<T>)make_shared() { return shared_ptr<T>(::new T()); }

template <typename T, typename A0>
FT_REQUIRES(!ft::is_array<T>::value)
(shared_ptr<T>)make_shared(const A0& arg0)
{
	return shared_ptr<T>(::new T(arg0));
}

template <typename T, typename A0, typename A1>
FT_REQUIRES(!ft::is_array<T>::value)
(shared_ptr<T>)make_shared(const A0& arg0, const A1& arg1)
{
	return shared_ptr<T>(::new T(arg0, arg1));
}

template <typename T, typename A0, typename A1, typename A2>
FT_REQUIRES(!ft::is_array<T>::value)
(shared_ptr<T>)make_shared(const A0& arg0, const A1& arg1, const A2& arg2)
{
	return shared_ptr<T>(::new T(arg0, arg1, arg2));
}

template <typename T, typename A0, typename A1, typename A2, typename A3>
FT_REQUIRES(!ft::is_array<T>::value)
(shared_ptr<T>)
    make_shared(const A0& arg0, const A1& arg1, const A2& arg2, const A3& arg3)
{
	return shared_ptr<T>(::new T(arg0, arg1, arg2, arg3));
}

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
                           const A4& arg4)
{
	return shared_ptr<T>(::new T(arg0, arg1, arg2, arg3, arg4));
}

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
                           const A5& arg5)
{
	return shared_ptr<T>(::new T(arg0, arg1, arg2, arg3, arg4, arg5));
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
(shared_ptr<T>)make_shared(const A0& arg0,
                           const A1& arg1,
                           const A2& arg2,
                           const A3& arg3,
                           const A4& arg4,
                           const A5& arg5,
                           const A6& arg6)
{
	return shared_ptr<T>(::new T(arg0, arg1, arg2, arg3, arg4, arg5, arg6));
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
(shared_ptr<T>)make_shared(const A0& arg0,
                           const A1& arg1,
                           const A2& arg2,
                           const A3& arg3,
                           const A4& arg4,
                           const A5& arg5,
                           const A6& arg6,
                           const A7& arg7)
{
	return shared_ptr<T>(
	    ::new T(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7));
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
(shared_ptr<T>)make_shared(const A0& arg0,
                           const A1& arg1,
                           const A2& arg2,
                           const A3& arg3,
                           const A4& arg4,
                           const A5& arg5,
                           const A6& arg6,
                           const A7& arg7,
                           const A8& arg8)
{
	return shared_ptr<T>(
	    ::new T(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8));
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
(shared_ptr<T>)make_shared(const A0& arg0,
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
	return shared_ptr<T>(
	    ::new T(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9));
}

template <typename T>
FT_REQUIRES(ft::is_unbounded_array<T>::value)
(shared_ptr<T>)make_shared(std::size_t N)
{
	return shared_ptr<T>(::new typename ft::remove_extent<T>::type[N]());
}

template <typename T>
FT_REQUIRES(ft::is_bounded_array<T>::value)
(shared_ptr<T>)make_shared() { return shared_ptr<T>(::new T()); }

template <typename T>
FT_REQUIRES(!ft::is_unbounded_array<T>::value)
(shared_ptr<T>)make_shared_for_overwrite() { return shared_ptr<T>(::new T); }

template <typename T>
FT_REQUIRES(ft::is_unbounded_array<T>::value)
(shared_ptr<T>)make_shared_for_overwrite(std::size_t N)
{
	return shared_ptr<T>(::new typename ft::remove_extent<T>::type[N]);
}

template <typename T, typename U>
shared_ptr<T> static_pointer_cast(const shared_ptr<U>& r) throw()
{
	typedef typename shared_ptr<T>::element_type Y;
	Y* const p = static_cast<Y*>(r.get());
	return shared_ptr<T>(r, p);
}

template <typename T, typename U>
shared_ptr<T> static_pointer_cast(ft::rvalue<shared_ptr<U> >& r) throw()
{
	typedef typename shared_ptr<T>::element_type Y;
	Y* const p = static_cast<Y*>(r.get());
	return shared_ptr<T>(ft::move(r), p);
}

template <typename T, typename U>
shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& r) throw()
{
	typedef typename shared_ptr<T>::element_type Y;
	if (Y* const p = dynamic_cast<Y*>(r.get())) {
		return shared_ptr<T>(r, p);
	}
	return shared_ptr<T>();
}

template <typename T, typename U>
shared_ptr<T> dynamic_pointer_cast(ft::rvalue<shared_ptr<U> >& r) throw()
{
	typedef typename shared_ptr<T>::element_type Y;
	if (Y* const p = dynamic_cast<Y*>(r.get())) {
		return shared_ptr<T>(ft::move(r), p);
	}
	return shared_ptr<T>();
}

template <typename T, typename U>
shared_ptr<T> const_pointer_cast(const shared_ptr<U>& r) throw()
{
	typedef typename shared_ptr<T>::element_type Y;
	Y* const p = const_cast<Y*>(r.get());
	return shared_ptr<T>(r, p);
}

template <typename T, typename U>
shared_ptr<T> const_pointer_cast(ft::rvalue<shared_ptr<U> >& r) throw()
{
	typedef typename shared_ptr<T>::element_type Y;
	Y* const p = const_cast<Y*>(r.get());
	return shared_ptr<T>(ft::move(r), p);
}

template <typename T, typename U>
shared_ptr<T> reinterpret_pointer_cast(const shared_ptr<U>& r) throw()
{
	typedef typename shared_ptr<T>::element_type Y;
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	Y* const p = reinterpret_cast<Y*>(r.get());
	return shared_ptr<T>(r, p);
}

template <typename T, typename U>
shared_ptr<T> reinterpret_pointer_cast(ft::rvalue<shared_ptr<U> >& r) throw()
{
	typedef typename shared_ptr<T>::element_type Y;
	// NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
	Y* const p = reinterpret_cast<Y*>(r.get());
	return shared_ptr<T>(ft::move(r), p);
}

template <typename Deleter, typename T>
Deleter* get_deleter(const shared_ptr<T>& p) throw()
{
	if (p._control == FT_NULLPTR) {
		return FT_NULLPTR;
	}
	return static_cast<Deleter*>(p._control->get_deleter(typeid(Deleter)));
}

template <typename T, typename U>
bool operator==(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) throw()
{
	return lhs.get() == rhs.get();
}

template <typename T, typename U>
bool operator!=(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) throw()
{
	return !(lhs == rhs);
}

template <typename T, typename U>
bool operator<(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) throw()
{
	return lhs.get() < rhs.get();
}

template <typename T, typename U>
bool operator>(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) throw()
{
	return rhs < lhs;
}

template <typename T, typename U>
bool operator<=(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) throw()
{
	return !(rhs < lhs);
}

template <typename T, typename U>
bool operator>=(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) throw()
{
	return !(lhs < rhs);
}

template <typename T>
bool operator==(const shared_ptr<T>& lhs, ft::nullptr_t /*unused*/) throw()
{
	return !lhs;
}

template <typename T>
bool operator==(ft::nullptr_t /*unused*/, const shared_ptr<T>& rhs) throw()
{
	return !rhs;
}

template <typename T>
bool operator!=(const shared_ptr<T>& lhs, ft::nullptr_t /*unused*/) throw()
{
	return static_cast<bool>(lhs);
}

template <typename T>
bool operator!=(ft::nullptr_t /*unused*/, const shared_ptr<T>& rhs) throw()
{
	return static_cast<bool>(rhs);
}

template <typename T>
bool operator<(const shared_ptr<T>& lhs, ft::nullptr_t /*unused*/) throw()
{
	return ft::less<typename shared_ptr<T>::element_type*>()(lhs.get(),
	                                                         FT_NULLPTR);
}

template <typename T>
bool operator<(ft::nullptr_t /*unused*/, const shared_ptr<T>& rhs) throw()
{
	return ft::less<typename shared_ptr<T>::element_type*>()(FT_NULLPTR,
	                                                         rhs.get());
}

template <typename T>
bool operator>(const shared_ptr<T>& lhs, ft::nullptr_t /*unused*/) throw()
{
	return FT_NULLPTR < lhs;
}

template <typename T>
bool operator>(ft::nullptr_t /*unused*/, const shared_ptr<T>& rhs) throw()
{
	return rhs < FT_NULLPTR;
}

template <typename T>
bool operator<=(const shared_ptr<T>& lhs, ft::nullptr_t /*unused*/) throw()
{
	return !(FT_NULLPTR < lhs);
}

template <typename T>
bool operator<=(ft::nullptr_t /*unused*/, const shared_ptr<T>& rhs) throw()
{
	return !(rhs < FT_NULLPTR);
}

template <typename T>
bool operator>=(const shared_ptr<T>& lhs, ft::nullptr_t /*unused*/) throw()
{
	return !(lhs < FT_NULLPTR);
}

template <typename T>
bool operator>=(ft::nullptr_t /*unused*/, const shared_ptr<T>& rhs) throw()
{
	return !(FT_NULLPTR < rhs);
}

template <typename T, typename U, typename V>
std::basic_ostream<U, V>& operator<<(std::basic_ostream<U, V>& os,
                                     const shared_ptr<T>& ptr)
{
	return os << ptr.get();
}

template <typename T>
void swap(shared_ptr<T>& lhs, shared_ptr<T>& rhs) throw()
{
	lhs.swap(rhs);
}

} // namespace ft

#endif // LIBFTPP_MEMORY_SHARED_PTR_TPP
