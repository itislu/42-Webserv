#pragma once
#ifndef ALTERNATIVE_RULE_HPP
#define ALTERNATIVE_RULE_HPP

#include "utils/BufferReader.hpp"
#include "utils/abnfRules/Rule.hpp"

#include <vector>

/* ************************************************************************** */
class AlternativeRule : public Rule
{
public:
  enum AlternativeMode
  {
    FirstMatchWins,
    Greedy // find longest match
  };

  AlternativeRule();
  ~AlternativeRule();

  bool matches();
  void reset();
  void setBufferReader(BufferReader* bufferReader);
  void setResultMap(ResultMap* results);

  void addRule(Rule* rule);

private:
  AlternativeRule(const AlternativeRule& other);
  AlternativeRule& operator=(const AlternativeRule& other);

  bool _firstMatchMode();
  bool _greedyMode();

  std::vector<Rule*> _rules;
  AlternativeMode _mode; // FirstMatchWins or Greedy
};

#endif
