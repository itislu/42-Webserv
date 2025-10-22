#pragma once
#ifndef RULE_RESULT_HPP
#define RULE_RESULT_HPP

/* ************************************************************************** */
class RuleResult
{
public:
  RuleResult();
  RuleResult(long start, long end);
  ~RuleResult();
  RuleResult(const RuleResult& other);
  RuleResult& operator=(const RuleResult& other);

  long getStart() const;
  long getEnd() const;

private:

  long _start;
  long _end;
};

#endif
