// IWYU pragma: private; include "libftpp/string.hpp"
#pragma once
#ifndef LIBFTPP_STRING_STARTS_WITH_TPP
#	define LIBFTPP_STRING_STARTS_WITH_TPP

#	include "libftpp/string.hpp"
#	include <cassert>
#	include <cstddef>
#	include <string>

namespace ft {

template <typename CharT, typename Traits, typename Allocator>
bool starts_with(const std::basic_string<CharT, Traits, Allocator>& str,
                 const std::basic_string<CharT, Traits, Allocator>& prefix)
{
	if (prefix.length() > str.length()) {
		return false;
	}
	return str.compare(0, prefix.length(), prefix) == 0;
}

template <typename CharT, typename Traits, typename Allocator>
bool starts_with(const std::basic_string<CharT, Traits, Allocator>& str,
                 CharT ch)
{
	return !str.empty() && str[0] == ch;
}

template <typename CharT, typename Traits, typename Allocator>
bool starts_with(const std::basic_string<CharT, Traits, Allocator>& str,
                 const CharT* prefix)
{
	assert(prefix != NULL);

	typename std::basic_string<CharT, Traits, Allocator>::size_type prefix_len =
	    Traits::length(prefix);
	if (prefix_len > str.length()) {
		return false;
	}
	return str.compare(0, prefix_len, prefix, prefix_len) == 0;
}

} // namespace ft

#endif // LIBFTPP_STRING_STARTS_WITH_TPP
