#pragma once
#ifndef LIBFTPP_SAFE_BOOL_SAFE_BOOL_BASE_HPP
#	define LIBFTPP_SAFE_BOOL_SAFE_BOOL_BASE_HPP

namespace ft {
namespace _safe_bool {

class safe_bool_base {
public:
	typedef void (safe_bool_base::*safe_bool_t)() const;

	void this_type_does_not_support_comparisons() const;

protected:
	safe_bool_base();
	~safe_bool_base();

private:
	safe_bool_base(const safe_bool_base& /*unused*/);
	safe_bool_base& operator=(const safe_bool_base& /*unused*/);
};

} // namespace _safe_bool
} // namespace ft

#endif // LIBFTPP_SAFE_BOOL_SAFE_BOOL_BASE_HPP
