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

RuleResult::~RuleResult() {}

RuleResult::RuleResult(const RuleResult& other)
  : _start(other._start)
  , _end(other._end)
{
  *this = other;
}

RuleResult& RuleResult::operator=(const RuleResult& other)
{
  if (this != &other) {
    _start = other._start;
    _end = other._end;
  }
  return *this;
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
