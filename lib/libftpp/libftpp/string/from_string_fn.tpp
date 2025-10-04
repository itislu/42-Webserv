// IWYU pragma: private; include "libftpp/string.hpp"
#pragma once
#ifndef LIBFTPP_STRING_FROM_STRING_FN_TPP
#	define LIBFTPP_STRING_FROM_STRING_FN_TPP

#	include "libftpp/string.hpp"
#	include <ios>
#	include <string>

namespace ft {

template <typename T>
T from_string_fn<T>::operator()(const std::string& str)
{
	return ft::from_string<T>(str);
}

template <typename T>
from_string_fmt_fn<T>::from_string_fmt_fn(std::ios::fmtflags fmt_)
    : fmt(fmt_)
{}

template <typename T>
T from_string_fmt_fn<T>::operator()(const std::string& str)
{
	return ft::from_string<T>(str, fmt);
}

} // namespace ft

#endif // LIBFTPP_STRING_FROM_STRING_FN_TPP
