// IWYU pragma: private; include "libftpp/movable.hpp"
#pragma once
#ifndef LIBFTPP_MOVABLE_MOVABLE_IPP
#	define LIBFTPP_MOVABLE_MOVABLE_IPP

#	define LIBFTPP_IMPLICIT_RVALUE_CONVERSION(TYPE)            \
		operator ft::rvalue<TYPE>&() throw()                    \
		{                                                       \
			return static_cast<ft::rvalue<TYPE>&>(*this);       \
		}                                                       \
		operator const ft::rvalue<TYPE>&() const throw()        \
		{                                                       \
			return static_cast<const ft::rvalue<TYPE>&>(*this); \
		}

// NOLINTBEGIN(bugprone-macro-parentheses): Not valid syntax.
// NOLINTBEGIN(bugprone-unhandled-self-assignment): Only forwarding.
// NOLINTBEGIN(misc-unconventional-assign-operator): Needed to inhibit implicit
// copy assignment operator generation with const ref parameter, which would
// have too high priority by overload resolution over rvalues.
#	define LIBFTPP_COPYABLE_AND_MOVABLE_IMPL(TYPE) \
	public:                                         \
		LIBFTPP_IMPLICIT_RVALUE_CONVERSION(TYPE)    \
	public:                                         \
		TYPE& operator=(TYPE& t)                    \
		{                                           \
			*this = const_cast<const TYPE&>(t);     \
			return *this;                           \
		}                                           \
                                                    \
	private:
// NOLINTEND(misc-unconventional-assign-operator)
// NOLINTEND(bugprone-unhandled-self-assignment)
// NOLINTEND(bugprone-macro-parentheses)

#	define LIBFTPP_COPYABLE_AND_MOVABLE_SWAP_IMPL(TYPE) \
	public:                                              \
		LIBFTPP_IMPLICIT_RVALUE_CONVERSION(TYPE)         \
                                                         \
	private:

// NOLINTBEGIN(bugprone-macro-parentheses): Not valid syntax.
#	define LIBFTPP_MOVABLE_BUT_NOT_COPYABLE_IMPL(TYPE)          \
	public:                                                      \
		LIBFTPP_IMPLICIT_RVALUE_CONVERSION(TYPE)                 \
	private:                                                     \
		TYPE(TYPE&);                                             \
		TYPE& operator=(TYPE&);                                  \
		TYPE& operator=(const ft::copy_assign_ref<TYPE>& other); \
                                                                 \
	private:
// NOLINTEND(bugprone-macro-parentheses)

#endif // LIBFTPP_MOVABLE_MOVABLE_IPP
