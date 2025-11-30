#pragma once
#ifndef SEQUENCE_RULE_HPP
#define SEQUENCE_RULE_HPP

#include "Rule.hpp"

#include <libftpp/memory.hpp>
#include <utils/buffer/IInBuffer.hpp>

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
  void setBufferReader(IInBuffer* bufferReader);
  void setResultMap(ResultMap* results);

  void addRule(ft::shared_ptr<Rule> rule);

private:
  SequenceRule(const SequenceRule& other);
  SequenceRule& operator=(const SequenceRule& other);

  void _setNextRule();
  static bool _isRepetitionRule(Rule* rule);
  static bool _isRepOrSeqRule(Rule* rule);
  bool _isLastRule() const;

  std::size_t _currRule;
  std::vector<ft::shared_ptr<Rule> > _rules;
};

#endif
