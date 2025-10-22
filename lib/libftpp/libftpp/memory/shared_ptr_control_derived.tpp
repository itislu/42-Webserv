// IWYU pragma: private; include "shared_ptr_detail.hpp"
#pragma once
#ifndef LIBFTPP_MEMORY_SHARED_PTR_CONTROL_DERIVED_TPP
#	define LIBFTPP_MEMORY_SHARED_PTR_CONTROL_DERIVED_TPP

#	include "shared_ptr_detail.hpp"
#	include "addressof.tpp" // NOLINT(misc-include-cleaner): Avoid circular
// include of libftpp/memory.hpp.
#	include "libftpp/utility.hpp"
#	include <typeinfo>

namespace ft {
namespace _shared_ptr {

/* control_block_pointer */

template <typename Yp, typename T>
control_block_pointer<Yp, T>::control_block_pointer(Yp ptr) throw()
    : _ptr(ptr)
{}

template <typename Yp, typename T>
void control_block_pointer<Yp, T>::dispose() throw()
{
	_shared_ptr::delete_ptr<T>(_ptr);
}

template <typename Yp, typename T>
void* control_block_pointer<Yp, T>::get_deleter(
    const std::type_info& /*unused*/) throw()
{
	return FT_NULLPTR;
}

/* control_block_pointer_deleter */

template <typename Yp, typename Deleter>
control_block_pointer_deleter<Yp, Deleter>::control_block_pointer_deleter(
    Yp ptr,
    Deleter d) throw()
    : _ptr(ptr),
      _deleter(d)
{}

template <typename Yp, typename Deleter>
void control_block_pointer_deleter<Yp, Deleter>::dispose() throw()
{
	_deleter(_ptr);
}

template <typename Yp, typename Deleter>
void* control_block_pointer_deleter<Yp, Deleter>::get_deleter(
    const std::type_info& t) throw()
{
	return t == typeid(Deleter) ? ft::addressof(_deleter) : FT_NULLPTR;
}

} // namespace _shared_ptr
} // namespace ft

#endif // LIBFTPP_MEMORY_SHARED_PTR_CONTROL_DERIVED_TPP
