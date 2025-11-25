// IWYU pragma: private; include "libftpp/string.hpp"
#pragma once
#ifndef LIBFTPP_STRING_FROM_STRING_TPP
#	define LIBFTPP_STRING_FROM_STRING_TPP

#	include "libftpp/ctype.hpp"
#	include "libftpp/expected.hpp"
#	include "libftpp/iterator.hpp"
#	include "libftpp/string.hpp"
#	include "libftpp/type_traits.hpp"
#	include "libftpp/utility.hpp"
#	include <algorithm>
#	include <cerrno>
#	include <cmath>
#	include <cstdlib>
#	include <iostream>
#	include <limits>
#	include <new>
#	include <sstream>
#	include <string>

namespace ft {

namespace _from_string {
template <typename To, typename = void>
struct impl {
	static To handle_success(To& res,
	                         const std::string& str,
	                         std::ios::fmtflags fmt,
	                         std::string::size_type& endpos_out);
	static To handle_error(To& res,
	                       const std::string& str,
	                       std::ios::fmtflags fmt,
	                       std::string::size_type& endpos_out);
};
} // namespace _from_string

template <typename To>
To from_string(const std::string& str,
               std::string::size_type* endpos_out /*= FT_NULLPTR*/)
{
	return ft::from_string<To>(str, std::ios::fmtflags(), endpos_out);
}

template <>
inline bool
from_string<bool>(const std::string& str,
                  std::string::size_type* endpos_out /*= FT_NULLPTR*/)
{
	try {
		return ft::from_string<bool>(str, std::ios::boolalpha, endpos_out);
	}
	catch (const from_string_exception&) {
		return ft::from_string<bool>(str, std::ios::fmtflags(), endpos_out);
	}
}

template <typename To>
To from_string(const std::string& str,
               std::ios::fmtflags fmt,
               std::string::size_type* endpos_out /*= FT_NULLPTR*/)
{
	// NOLINTNEXTLINE: Let endpos always be a valid reference
	std::string::size_type _, &endpos = (endpos_out ? *endpos_out : _) = 0;
	To res;
	std::istringstream iss(str);
	iss.flags(fmt);

	if (iss >> res) {
		endpos = std::min(static_cast<std::string::size_type>(iss.tellg()),
		                  str.length());
		return _from_string::impl<To>::handle_success(res, str, fmt, endpos);
	}

	/* Error handling */
	if (ft::is_same<To, bool>::value && fmt & std::ios::boolalpha) {
		throw from_string_invalid_exception(str, typeid(To));
	}
	if (!(fmt & std::ios::skipws) && ft::isspace(*str.c_str())) {
		throw from_string_invalid_exception(str, typeid(To));
	}
	return _from_string::impl<To>::handle_error(res, str, fmt, endpos);
}

template <typename To>
ft::expected<To, ft::from_string_exception>
from_string(const std::string& str,
            std::nothrow_t /*nothrow*/,
            std::string::size_type* endpos_out /*= FT_NULLPTR*/)
{
	try {
		return ft::from_string<To>(str, endpos_out);
	}
	catch (const from_string_exception& e) {
		return ft::unexpected<from_string_exception>(e);
	}
}

template <typename To>
ft::expected<To, ft::from_string_exception>
from_string(const std::string& str,
            std::ios::fmtflags fmt,
            std::nothrow_t /*nothrow*/,
            std::string::size_type* endpos_out /*= FT_NULLPTR*/)
{
	try {
		return ft::from_string<To>(str, fmt, endpos_out);
	}
	catch (const from_string_exception& e) {
		return ft::unexpected<from_string_exception>(e);
	}
}

namespace _from_string {

template <typename To, typename Default /*= void*/>
To impl<To, Default>::handle_success(To& res,
                                     const std::string& /*unused*/,
                                     std::ios::fmtflags /*unused*/,
                                     std::string::size_type& /*unused*/)
{
	return res;
}

template <typename To, typename Default /*= void*/>
To impl<To, Default>::handle_error(To& /*unused*/,
                                   const std::string& str,
                                   std::ios::fmtflags /*unused*/,
                                   std::string::size_type& /*unused*/)
{
	throw from_string_invalid_exception(str, typeid(To));
}

/* integral types */
template <typename To>
struct impl<To, typename ft::enable_if<ft::is_integral<To>::value>::type> {
	static To handle_success(To& res,
	                         const std::string& str,
	                         std::ios::fmtflags /*unused*/,
	                         std::string::size_type& endpos_out)
	{
		// stringstream wraps negative values for unsigend integer types
		_check_negative_unsigned_integral(str, endpos_out);
		return res;
	}

	static To handle_error(To& /*unused*/,
	                       const std::string& str,
	                       std::ios::fmtflags /*unused*/,
	                       std::string::size_type& endpos_out)
	{
		const char* const start = str.c_str();
		// False positive: Check does not run on C code (`strto*` signature).
		char* end = FT_NULLPTR; // NOLINT(misc-const-correctness)

		// strtol works even for unsigned long since valid numbers between
		// LONG_MAX and ULONG_MAX would not get here
		int errno_local = errno;
		errno = 0;
		const long test = std::strtol(start, &end, 0);
		std::swap(errno, errno_local);
		if (errno_local == ERANGE
		    || test < static_cast<long>(std::numeric_limits<To>::min())
		    || static_cast<unsigned long>(test) > static_cast<unsigned long>(
		           std::numeric_limits<To>::max())) {
			if (end != FT_NULLPTR) {
				endpos_out = end - start;
			}
			throw from_string_range_exception(str, typeid(To));
		}
		throw from_string_invalid_exception(str, typeid(To));
	}

private:
	static void _check_negative_unsigned_integral(const std::string& str,
	                                              std::string::size_type endpos)
	{
		if (ft::is_signed<To>::value || ft::is_same<To, bool>::value) {
			return;
		}

		const std::string::const_iterator end =
		    str.begin() + static_cast<std::string::difference_type>(endpos);
		const std::string::const_iterator minus =
		    std::find(str.begin(), end, '-');
		if (minus == end) {
			return;
		}
		if (std::find_first_of(
		        minus, end, ft::begin(_not_zero), ft::prev(ft::end(_not_zero)))
		    != end) {
			throw from_string_range_exception(str, typeid(To));
		}
	}

	static const char _not_zero[];
};

template <typename To>
const char impl<To, typename ft::enable_if<ft::is_integral<To>::value>::type>::
    _not_zero[] = "123456789abcdefABCDEF";

/* floating point types */
template <typename To>
struct impl<To,
            typename ft::enable_if<ft::is_floating_point<To>::value>::type> {
	static To handle_success(To& res,
	                         const std::string& str,
	                         std::ios::fmtflags fmt,
	                         std::string::size_type& endpos_out)
	{
		_check_unwanted_scientific_notation(str, fmt, endpos_out);
		return res;
	}

	static To handle_error(To& res,
	                       const std::string& str,
	                       std::ios::fmtflags fmt,
	                       std::string::size_type& endpos_out)
	{
		const char* const start = str.c_str();
		// False positive: Check does not run on C code (`strto*` signature).
		char* end = FT_NULLPTR; // NOLINT(misc-const-correctness)

		int errno_local = errno;
		errno = 0;
		if (ft::is_same<To, float>::value) {
			res = std::strtof(start, &end);
		}
		else if (ft::is_same<To, double>::value) {
			res = std::strtod(start, &end);
		}
		else {
			res = std::strtold(start, &end);
		}
		std::swap(errno, errno_local);
		if (end != FT_NULLPTR) {
			endpos_out = end - start;
		}

		_check_unwanted_scientific_notation(str, fmt, endpos_out);
		if (errno_local == ERANGE) {
			throw from_string_range_exception(str, typeid(To));
		}
		// stringstream does not detect special floating point values
		if (std::isinf(res) || std::isnan(res)) {
			return res;
		}
		throw from_string_invalid_exception(str, typeid(To));
	}

private:
	static void
	_check_unwanted_scientific_notation(const std::string& str,
	                                    std::ios::fmtflags fmt,
	                                    std::string::size_type& endpos_out)
	{
		if ((fmt & std::ios::floatfield) != std::ios::fixed) {
			return;
		}

		const std::string::const_iterator end =
		    str.begin() + static_cast<std::string::difference_type>(endpos_out);
		if (std::find_first_of(str.begin(),
		                       end,
		                       ft::begin(_scientific_notation),
		                       ft::prev(ft::end(_scientific_notation)))
		    != end) {
			endpos_out = 0; // For consistency with other invalid cases
			throw from_string_invalid_exception(str, typeid(To));
		}
	}

	static const char _scientific_notation[];
};

template <typename To>
const char
    impl<To, typename ft::enable_if<ft::is_floating_point<To>::value>::type>::
        _scientific_notation[] = "eEpP";

} // namespace _from_string

} // namespace ft

#endif // LIBFTPP_STRING_FROM_STRING_TPP
