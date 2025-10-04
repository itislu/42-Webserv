// IWYU pragma: private; include "libftpp/format.hpp"
#pragma once
#ifndef LIBFTPP_FORMAT_LOG_TPP
#	define LIBFTPP_FORMAT_LOG_TPP

#	include "libftpp/format.hpp"
#	include <iomanip>
#	include <ios>
#	include <sstream>
#	include <string>

namespace ft {
namespace log {

template <typename T>
std::string ok(const T& msg)
{
	std::ostringstream os;
	os << std::left << std::setw(label_width_colored)
	   << ft::bold(ft::green("[OK]")) << msg;
	return os.str();
}

template <typename T>
std::string info(const T& msg)
{
	std::ostringstream os;
	os << std::left << std::setw(label_width_colored)
	   << ft::bold(ft::blue("[INFO]")) << msg;
	return os.str();
}

template <typename T>
std::string warn(const T& msg)
{
	std::ostringstream os;
	os << std::left << std::setw(label_width_colored)
	   << ft::bold(ft::yellow("[WARN]")) << msg;
	return os.str();
}

template <typename T>
std::string error(const T& msg)
{
	std::ostringstream os;
	os << std::left << std::setw(label_width_colored)
	   << ft::bold(ft::red("[ERROR]")) << msg;
	return os.str();
}

template <typename T>
std::string line(const T& msg)
{
	std::ostringstream os;
	os << '\n' << std::left << std::setw(label_width_text) << "" << msg;
	return os.str();
}

} // namespace log
} // namespace ft

#endif // LIBFTPP_FORMAT_LOG_TPP
