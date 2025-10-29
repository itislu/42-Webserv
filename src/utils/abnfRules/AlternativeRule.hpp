#pragma once
#ifndef ALTERNATIVE_RULE_HPP
#define ALTERNATIVE_RULE_HPP

#include <libftpp/memory.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/Rule.hpp>

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

  void addRule(ft::unique_ptr<Rule> rule);

private:
  AlternativeRule(const AlternativeRule& other);
  AlternativeRule& operator=(const AlternativeRule& other);

  bool _firstMatchMode();
  bool _greedyMode();

  std::vector<ft::shared_ptr<Rule> > _rules;
  AlternativeMode _mode; // FirstMatchWins or Greedy
};

#endif
