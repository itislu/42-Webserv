#include "libftpp/memory.hpp"
#include "libftpp/utility.hpp"
#include <string>

#if defined(__GNUC__) || defined(__clang__)
#	include <cstdlib>
#	include <cxxabi.h>
#	define LIBFTPP_HAVE_CXXABI_H
#endif // defined(__GNUC__) || defined(__clang__)

namespace ft {

std::string demangle(const char* mangled_name)
{
#ifdef LIBFTPP_HAVE_CXXABI_H
	int status = 0;
	const ft::unique_ptr<char, void (*)(void*)> demangled(
	    abi::__cxa_demangle(mangled_name, FT_NULLPTR, FT_NULLPTR, &status),
	    std::free);
	return status == 0 ? demangled.get() : mangled_name;
#else  // LIBFTPP_HAVE_CXXABI_H
	return mangled_name;
#endif // LIBFTPP_HAVE_CXXABI_H
}

} // namespace ft
