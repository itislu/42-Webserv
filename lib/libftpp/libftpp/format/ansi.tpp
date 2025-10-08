// IWYU pragma: private; include "libftpp/format.hpp"
#pragma once
#ifndef LIBFTPP_FORMAT_ANSI_TPP
#	define LIBFTPP_FORMAT_ANSI_TPP

#	include "libftpp/format.hpp"
#	include <sstream>
#	include <string>

namespace ft {

namespace _color {
template <typename T>
std::string apply_ansi(const T& msg, const char* style_code);
} // namespace _color

template <typename T>
std::string bold(const T& msg)
{
	return _color::apply_ansi(msg, "1");
}

template <typename T>
std::string italic(const T& msg)
{
	return _color::apply_ansi(msg, "3");
}

template <typename T>
std::string underline(const T& msg)
{
	return _color::apply_ansi(msg, "4");
}

template <typename T>
std::string red(const T& msg)
{
	return _color::apply_ansi(msg, "31");
}

template <typename T>
std::string green(const T& msg)
{
	return _color::apply_ansi(msg, "32");
}

template <typename T>
std::string yellow(const T& msg)
{
	return _color::apply_ansi(msg, "33");
}

template <typename T>
std::string blue(const T& msg)
{
	return _color::apply_ansi(msg, "34");
}

template <typename T>
std::string magenta(const T& msg)
{
	return _color::apply_ansi(msg, "35");
}

template <typename T>
std::string cyan(const T& msg)
{
	return _color::apply_ansi(msg, "36");
}

template <typename T>
std::string gray(const T& msg)
{
	return _color::apply_ansi(msg, "37");
}

namespace _color {

template <typename T>
std::string apply_ansi(const T& msg, const char* style_code)
{
	const char* const esc_start = "\033[";
	const char* const esc_end = "m";
	const char* const reset = "\033[0m";

	std::ostringstream os;
	os << esc_start << style_code << esc_end << msg << reset;
	return os.str();
}

} // namespace _color

} // namespace ft

#endif // LIBFTPP_FORMAT_ANSI_TPP
