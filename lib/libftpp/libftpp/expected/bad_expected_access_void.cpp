// IWYU pragma: private; include "libftpp/expected.hpp"
#include "libftpp/expected.hpp"

namespace ft {

const char* bad_expected_access<void>::what() const throw()
{
	return "bad access to ft::expected without expected value";
}

bad_expected_access<void>::bad_expected_access() throw() {}

bad_expected_access<void>::bad_expected_access(
    const bad_expected_access& /*unused*/)
{}

bad_expected_access<void>::~bad_expected_access() throw() {}

bad_expected_access<void>&
bad_expected_access<void>::operator=(const bad_expected_access& /*unused*/)
{
	return *this;
}

} // namespace ft
