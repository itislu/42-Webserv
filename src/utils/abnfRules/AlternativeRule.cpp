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

void AlternativeRule::setMatchMode(AlternativeMode mode)
{
  _mode = mode;
}

/* ************************************************************************** */
// PRIVATE

bool AlternativeRule::_firstMatchMode()
{
  setStartPos(getBuffReader()->getPosInBuff());
  setEndPos(-1);
  bool matches = false;
  for (std::size_t i = 0; i < _rules.size(); i++) {
    matches = _rules[i]->matches();
    if (matches) {
      setReachedEnd(_rules[i]->reachedEnd());
      setEndPos(getBuffReader()->getPosInBuff());
      break;
    }
    if (getBuffReader()->fail()) {
      break;
    }
    rewindToStartPos();
  }
  return matches;
}

bool AlternativeRule::_greedyMode()
{
  setStartPos(getBuffReader()->getPosInBuff());
  setEndPos(0);
  bool somethingMatched = false;
  for (std::size_t i = 0; i < _rules.size(); i++) {
    if (_rules[i]->matches()) {
      if (!somethingMatched || getBuffReader()->getPosInBuff() > getEndPos()) {
        setReachedEnd(_rules[i]->reachedEnd());
        setEndPos(getBuffReader()->getPosInBuff());
      }
      somethingMatched = true;
    }
    if (getBuffReader()->fail()) {
      break;
    }
    rewindToStartPos();
  }
  if (!somethingMatched) {
    return false;
  }
  moveToEndPos();
  return true;
}
