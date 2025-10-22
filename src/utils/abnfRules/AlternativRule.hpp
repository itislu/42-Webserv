
#pragma once
#ifndef ALTERNATIVE_RULE_HPP
#define ALTERNATIVE_RULE_HPP

#include "utils/abnfRules/Rule.hpp"
#include "utils/BufferReader.hpp"

#include <vector>

/* ************************************************************************** */
class AlternativRule : public Rule
{
public:
  enum AlternativMode
  {
    FirstMatchWins,
    Greedy // find longest match
  };

  AlternativRule();
  ~AlternativRule();

  bool matches();
  void reset();
  void setBufferReader(BufferReader* bufferReader);
  void setResultMap(ResultMap* results);

  void addRule(Rule* rule);

private:
  AlternativRule(const AlternativRule& other);
  AlternativRule& operator=(const AlternativRule& other);

  bool _firstMatchMode();
  bool _greedyMode();

  std::vector<Rule*> _rules;
  int _mode; // FirstMatchWins or Greedy
};

#endif // OPTION_RULE_HPP
