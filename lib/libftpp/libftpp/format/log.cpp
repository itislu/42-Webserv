#include "libftpp/format.hpp"
#include <string>

namespace ft {
namespace log {

std::string ok() { return ft::log::ok(""); }

std::string info() { return ft::log::info(""); }

std::string warn() { return ft::log::warn(""); }

std::string error() { return ft::log::error(""); }

std::string line() { return ft::log::line(""); }

} // namespace log
} // namespace ft
