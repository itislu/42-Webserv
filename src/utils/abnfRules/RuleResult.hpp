#pragma once
#ifndef RULE_RESULT_HPP
#define RULE_RESULT_HPP

#include <cstddef>

/* ************************************************************************** */
class RuleResult
{
public:
  RuleResult();
  RuleResult(std::size_t start, std::size_t end);
  std::size_t getStart() const;
  std::size_t getEnd() const;

private:
  std::size_t _start;
  std::size_t _end;
};

#endif
