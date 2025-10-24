#include "shared_ptr_detail.hpp"

namespace ft {
namespace _shared_ptr {

control_block_base::~control_block_base() {}

long control_block_base::use_count() const throw() { return _use_count; }

void control_block_base::add_shared() throw() { ++_use_count; }

bool control_block_base::release() throw()
{
	if (--_use_count != 0) {
		return false;
	}
	dispose();
	return true;
}

control_block_base::control_block_base() throw()
    : _use_count(1)
{}

} // namespace _shared_ptr
} // namespace ft
