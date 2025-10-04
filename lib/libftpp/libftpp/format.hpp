#pragma once
#ifndef LIBFTPP_FORMAT_HPP
#	define LIBFTPP_FORMAT_HPP

#	include <cstddef>
#	include <string>

#	define FT_BOLD(MSG)      (std::string("\033[1m") + MSG + "\033[0m")
#	define FT_ITALIC(MSG)    (std::string("\033[3m") + MSG + "\033[0m")
#	define FT_UNDERLINE(MSG) (std::string("\033[4m") + MSG + "\033[0m")
#	define FT_RED(MSG)       (std::string("\033[31m") + MSG + "\033[0m")
#	define FT_GREEN(MSG)     (std::string("\033[32m") + MSG + "\033[0m")
#	define FT_YELLOW(MSG)    (std::string("\033[33m") + MSG + "\033[0m")
#	define FT_BLUE(MSG)      (std::string("\033[34m") + MSG + "\033[0m")
#	define FT_MAGENTA(MSG)   (std::string("\033[35m") + MSG + "\033[0m")
#	define FT_CYAN(MSG)      (std::string("\033[36m") + MSG + "\033[0m")
#	define FT_GRAY(MSG)      (std::string("\033[37m") + MSG + "\033[0m")

namespace ft {

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

#	include "libftpp/format/log.tpp" // IWYU pragma: export

#endif // LIBFTPP_FORMAT_HPP
