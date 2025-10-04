#include "libftpp/exception.hpp"
#include "libftpp/numeric.hpp"
#include <string>

namespace ft {

numeric_cast_exception::numeric_cast_exception()
    : ft::exception("bad numeric conversion", "ft::numeric_cast")
{}

numeric_cast_exception::numeric_cast_exception(const std::string& msg)
    : ft::exception(msg, "ft::numeric_cast")
{}

numeric_cast_negative_overflow_exception::
    numeric_cast_negative_overflow_exception()
    : numeric_cast_exception("bad numeric conversion: negative overflow")
{}

numeric_cast_positive_overflow_exception::
    numeric_cast_positive_overflow_exception()
    : numeric_cast_exception("bad numeric conversion: positive overflow")
{}

} // namespace ft
