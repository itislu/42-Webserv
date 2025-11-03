#include "SequenceRule.hpp"

#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>
#include <utils/abnfRules/Rule.hpp>

#include <cstddef>

/* ************************************************************************** */
// PUBLIC

SequenceRule::SequenceRule()
  : _currRule(0)
{
  setDebugTag("Sequence");
}

SequenceRule::~SequenceRule() {}

bool SequenceRule::matches()
{
  debugPrintRuleEntry();
  setStartPos(getBuffReader()->getPosInBuff());
  bool matches = true;
  while (matches && !getBuffReader()->reachedEnd()) {

    _rules[_currRule]->setDebugPrintIndent(getDebugPrintIndent() + 2);
    matches = _rules[_currRule]->matches();

    if (matches) {
      _setNextRule();
    }

    if (_currRule >= _rules.size()) {
      setReachedEnd(true);
      break;
    }
  }

  if (getBuffReader()->reachedEnd() && !end()) {
    setDebugMatchReason("end of buffer; not end of seq");
  }

  setEndPos(getBuffReader()->getPosInBuff());
  addRuleResult(matches);
  debugPrintMatchStatus(matches);
  return matches;
}

void SequenceRule::reset()
{
  for (std::size_t i = 0; i < _rules.size(); i++) {
    _rules[i]->reset();
  }
  _currRule = 0;
  setReachedEnd(false);
}

void SequenceRule::setBufferReader(BufferReader* bufferReader)
{
  Rule::setBufferReader(bufferReader);
  for (std::size_t i = 0; i < _rules.size(); i++) {
    _rules[i]->setBufferReader(bufferReader);
  }
}

void SequenceRule::setResultMap(ResultMap* results)
{
  Rule::setResultMap(results);
  for (std::size_t i = 0; i < _rules.size(); i++) {
    _rules[i]->setResultMap(results);
  }
}

void SequenceRule::addRule(ft::shared_ptr<Rule> rule)
{
  _rules.push_back(ft::move(rule));
}

/* ************************************************************************** */
// PRIVATE

void SequenceRule::_setNextRule()
{
  // the current rule has to be at the end to go to the next rule
  if (!_rules[_currRule]->end()) {
    return;
  }

  if (_currRule < _rules.size()) {
    _currRule++;
  }
}

bool SequenceRule::_isRepetitionRule(Rule* rule)
{
  return dynamic_cast<RepetitionRule*>(rule) != FT_NULLPTR;
}

bool SequenceRule::_isRepOrSeqRule(Rule* rule)
{
  return dynamic_cast<RepetitionRule*>(rule) != FT_NULLPTR ||
         dynamic_cast<SequenceRule*>(rule) != FT_NULLPTR;
}

bool SequenceRule::_isLastRule() const
{
  return _currRule >= _rules.size() - 1;
}
