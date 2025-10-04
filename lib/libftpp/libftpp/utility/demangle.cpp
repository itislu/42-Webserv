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
	std::string result;
	int status = 0;
	char* const demangled =
	    abi::__cxa_demangle(mangled_name, NULL, NULL, &status);

	result = status == 0 ? demangled : mangled_name;
	std::free(demangled); // NOLINT(cppcoreguidelines-no-malloc)
	return result;
#else  // LIBFTPP_HAVE_CXXABI_H
	return mangled_name;
#endif // LIBFTPP_HAVE_CXXABI_H
}

} // namespace ft
