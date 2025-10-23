#pragma once
#ifndef SEQUENCE_RULE_HPP
#define SEQUENCE_RULE_HPP

#include "Rule.hpp"

#include "utils/BufferReader.hpp"

#include <cstddef>
#include <vector>

/* ************************************************************************** */
class SequenceRule : public Rule
{
public:
  SequenceRule();
  ~SequenceRule();

  bool matches();
  void reset();
  void setBufferReader(BufferReader* bufferReader);
  void setResultMap(ResultMap* results);

  void addRule(Rule* rule);

private:
  SequenceRule(const SequenceRule& other);
  SequenceRule& operator=(const SequenceRule& other);

  void _setNextRule();
  static bool _isRepetitionRule(Rule* rule);
  static bool _isRepOrSeqRule(Rule* rule);
  bool _isLastRule() const;

  std::size_t _currRule;
  std::vector<Rule*> _rules;
};

#endif
