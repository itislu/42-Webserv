#include "RuleResult.hpp"

/* ************************************************************************** */
// PUBLIC
RuleResult::RuleResult()
  : _start(-1)
  , _end(-1)
{
}

RuleResult::RuleResult(long start, long end)
  : _start(start)
  , _end(end)
{
}

long RuleResult::getStart() const
{
  return _start;
}
long RuleResult::getEnd() const
{
  return _end;
}

/* ************************************************************************** */
// PRIVATE
