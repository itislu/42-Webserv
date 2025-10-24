// IWYU pragma: private; include "libftpp/memory.hpp"
#pragma once
#ifndef LIBFTPP_MEMORY_OWNER_COMPARISON_TPP
#	define LIBFTPP_MEMORY_OWNER_COMPARISON_TPP

#	include "libftpp/memory.hpp"

namespace ft {

template <typename T>
bool owner_less<shared_ptr<T> >::operator()(const shared_ptr<T>& lhs,
                                            const shared_ptr<T>& rhs) const
    throw()
{
	return lhs.owner_before(rhs);
}

template <typename T, typename U>
bool owner_less<void>::operator()(const shared_ptr<T>& lhs,
                                  const shared_ptr<U>& rhs) const throw()
{
	return lhs.owner_before(rhs);
}

template <typename T, typename U>
bool owner_equal::operator()(const shared_ptr<T>& lhs,
                             const shared_ptr<U>& rhs) const throw()
{
	return lhs.owner_equal(rhs);
}

} // namespace ft

#endif // LIBFTPP_MEMORY_OWNER_COMPARISON_TPP
