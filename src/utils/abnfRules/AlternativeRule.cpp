#include "AlternativeRule.hpp"

#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/Rule.hpp>

#include <cstddef>

/* ************************************************************************** */
// PUBLIC

AlternativeRule::AlternativeRule()
  : _mode(Greedy)
{
  setDebugTag("Alternative");
}

AlternativeRule::~AlternativeRule() {}

bool AlternativeRule::matches()
{
  debugPrintRuleEntry();

  bool matches = false;
  switch (_mode) {
    case FirstMatchWins:
      matches = _firstMatchMode();
      break;
    case Greedy:
      matches = _greedyMode();
      break;
  }

  addRuleResult(matches);
  debugPrintMatchStatus(matches);
  return matches;
}

void AlternativeRule::reset()
{
  for (std::size_t i = 0; i < _rules.size(); i++) {
    _rules[i]->reset();
  }
  setReachedEnd(false);
}

void AlternativeRule::setBufferReader(BufferReader* bufferReader)
{
  Rule::setBufferReader(bufferReader);
  for (std::size_t i = 0; i < _rules.size(); i++) {
    _rules[i]->setBufferReader(bufferReader);
  }
}

void AlternativeRule::setResultMap(ResultMap* results)
{
  Rule::setResultMap(results);
  for (std::size_t i = 0; i < _rules.size(); i++) {
    _rules[i]->setResultMap(results);
  }
}

void AlternativeRule::addRule(ft::shared_ptr<Rule> rule)
{
  _rules.push_back(ft::move(rule));
}

/* ************************************************************************** */
// PRIVATE

bool AlternativeRule::_firstMatchMode()
{
  setStartPos(getBuffReader()->getPosInBuff());
  bool matches = false;
  for (std::size_t i = 0; i < _rules.size(); i++) {

    _rules[i]->setDebugPrintIndent(getDebugPrintIndent() + 2);

    matches = _rules[i]->matches();
    if (matches) {
      break;
    }
    rewindToStartPos();
  }
  return matches;
}

bool AlternativeRule::_greedyMode()
{
  setStartPos(getBuffReader()->getPosInBuff());
  setEndPos(-1);
  bool somethingMatched = false;
  for (std::size_t i = 0; i < _rules.size(); i++) {

    _rules[i]->setDebugPrintIndent(getDebugPrintIndent() + 2);

    if (_rules[i]->matches()) {
      // Is longer match or first match?
      if (getBuffReader()->getPosInBuff() > getEndPos() || !somethingMatched) {
        setReachedEnd(_rules[i]->reachedEnd());
        setEndPos(getBuffReader()->getPosInBuff());
      }
      somethingMatched = true;
    }
    rewindToStartPos();
  }
  if (!somethingMatched) {
    return false;
  }
  moveToEndPos();
  return true;
}
