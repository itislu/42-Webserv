#include "libftpp/optional.hpp"

namespace ft {

const char* bad_optional_access::what() const throw()
{
	return "bad optional access";
}

} // namespace ft
