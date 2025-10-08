#pragma once
#ifndef LIBFTPP_FORMAT_HPP
#	define LIBFTPP_FORMAT_HPP

#	include <cstddef>
#	include <string>

namespace ft {

template <typename T>
std::string bold(const T& msg);
template <typename T>
std::string italic(const T& msg);
template <typename T>
std::string underline(const T& msg);
template <typename T>
std::string red(const T& msg);
template <typename T>
std::string green(const T& msg);
template <typename T>
std::string yellow(const T& msg);
template <typename T>
std::string blue(const T& msg);
template <typename T>
std::string magenta(const T& msg);
template <typename T>
std::string cyan(const T& msg);
template <typename T>
std::string gray(const T& msg);

namespace log {

const std::size_t label_width_text = 8;
const std::size_t label_width_colored = 25;

template <typename T>
std::string ok(const T& msg);
std::string ok();

template <typename T>
std::string info(const T& msg);
std::string info();

template <typename T>
std::string warn(const T& msg);
std::string warn();

template <typename T>
std::string error(const T& msg);
std::string error();

template <typename T>
std::string line(const T& msg);
std::string line();

} // namespace log

} // namespace ft

#	include "libftpp/format/ansi.tpp" // IWYU pragma: export
#	include "libftpp/format/log.tpp"  // IWYU pragma: export

#endif // LIBFTPP_FORMAT_HPP
