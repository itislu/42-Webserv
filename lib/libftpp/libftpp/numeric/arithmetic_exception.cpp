#include "libftpp/exception.hpp"
#include "libftpp/numeric.hpp"
#include <string>

namespace ft {

arithmetic_exception::arithmetic_exception(const std::string& msg,
                                           const std::string& who)
    : ft::exception("arithmetic exception: " + msg, who)
{}

arithmetic_negative_overflow_exception::arithmetic_negative_overflow_exception(
    const std::string& who)
    : arithmetic_exception("negative overflow", who)
{}

arithmetic_positive_overflow_exception::arithmetic_positive_overflow_exception(
    const std::string& who)
    : arithmetic_exception("positive overflow", who)
{}

arithmetic_division_by_zero_exception::arithmetic_division_by_zero_exception(
    const std::string& who)
    : arithmetic_exception("division by zero", who)
{}

} // namespace ft
