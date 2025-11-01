#pragma once
#ifndef REPETITION_RULE_HPP
#define REPETITION_RULE_HPP

#include "Rule.hpp"

#include <libftpp/memory.hpp>
#include <utils/BufferReader.hpp>

/* ************************************************************************** */
class RepetitionRule : public Rule
{
public:
  explicit RepetitionRule(ft::shared_ptr<Rule> rule);
  ~RepetitionRule();

  bool matches();
  void reset();
  void setBufferReader(BufferReader* bufferReader);
  void setResultMap(ResultMap* results);

  void setMin(int value);
  void setMax(int value);
  bool reachedMin() const;
  int getReps() const;

private:
  RepetitionRule();
  RepetitionRule(const RepetitionRule& other);
  RepetitionRule& operator=(const RepetitionRule& other);

  void _incrementReps();

  int _minReps;
  int _maxReps;
  int _currReps;
  bool _reachedMin;
  ft::shared_ptr<Rule> _rule;
};

#endif // REPETITION_RULE_HPP
