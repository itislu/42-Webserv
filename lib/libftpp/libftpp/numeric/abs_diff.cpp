#include "libftpp/numeric.hpp"

namespace ft {

namespace _abs_diff {
template <typename U, typename I>
static U abs_diff(I a, I b) throw();
} // namespace _abs_diff

unsigned char abs_diff(char a, char b) throw()
{
	return _abs_diff::abs_diff<unsigned char>(a, b);
}

unsigned short abs_diff(short a, short b) throw()
{
	return _abs_diff::abs_diff<unsigned short>(a, b);
}

unsigned int abs_diff(int a, int b) throw()
{
	return _abs_diff::abs_diff<unsigned int>(a, b);
}

unsigned long abs_diff(long a, long b) throw()
{
	return _abs_diff::abs_diff<unsigned long>(a, b);
}

namespace _abs_diff {

template <typename U, typename I>
static U abs_diff(I a, I b) throw()
{
	// Same sign - subtract higher from lower.
	if ((a >= 0 && b >= 0) || (a < 0 && b < 0)) {
		return a > b ? a - b : b - a;
	}
	/*
	0. Higher nbr is known to be positive or 0, lower negative.
	1. Cast known positive nbr to unsigned to make all additions to the total
	   unsigned.
	2. Add 1 to negative nbr (losing 1 diff) to avoid overflow in next step.
	3. Make result of previous step positive and unsigned-add to total.
	4. Add back the lost diff to total.
	*/
	return a > b ? static_cast<U>(a) + -(b + 1) + 1
	             : static_cast<U>(b) + -(a + 1) + 1;
}

} // namespace _abs_diff

} // namespace ft
