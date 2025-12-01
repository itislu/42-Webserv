#include "RuleResult.hpp"

#include <cstddef>

/* ************************************************************************** */
// PUBLIC
RuleResult::RuleResult()
  : _start(-1)
  , _end(-1)
{
}

RuleResult::RuleResult(std::size_t start, std::size_t end)
  : _start(start)
  , _end(end)
{
}

std::size_t RuleResult::getStart() const
{
  return _start;
}
std::size_t RuleResult::getEnd() const
{
  return _end;
}

/* ************************************************************************** */
// PRIVATE
