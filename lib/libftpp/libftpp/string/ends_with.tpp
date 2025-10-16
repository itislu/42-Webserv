// IWYU pragma: private; include "libftpp/string.hpp"
#pragma once
#ifndef LIBFTPP_STRING_ENDS_WITH_TPP
#	define LIBFTPP_STRING_ENDS_WITH_TPP

#	include "libftpp/string.hpp"
#	include "libftpp/utility.hpp"
#	include <cassert>
#	include <string>

namespace ft {

template <typename CharT, typename Traits, typename Allocator>
bool ends_with(const std::basic_string<CharT, Traits, Allocator>& str,
               const std::basic_string<CharT, Traits, Allocator>& suffix)
{
	if (suffix.length() > str.length()) {
		return false;
	}
	return str.compare(str.length() - suffix.length(), suffix.length(), suffix)
	       == 0;
}

template <typename CharT, typename Traits, typename Allocator>
bool ends_with(const std::basic_string<CharT, Traits, Allocator>& str, CharT ch)
{
	return !str.empty() && str[str.length() - 1] == ch;
}

template <typename CharT, typename Traits, typename Allocator>
bool ends_with(const std::basic_string<CharT, Traits, Allocator>& str,
               const CharT* suffix)
{
	assert(suffix != FT_NULLPTR);

	typename std::basic_string<CharT, Traits, Allocator>::size_type suffix_len =
	    Traits::length(suffix);
	if (suffix_len > str.length()) {
		return false;
	}
	return str.compare(
	           str.length() - suffix_len, suffix_len, suffix, suffix_len)
	       == 0;
}

} // namespace ft

#endif // LIBFTPP_STRING_ENDS_WITH_TPP
