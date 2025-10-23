#include "AlternativeRule.hpp"

#include <utils/BufferReader.hpp>
#include <utils/abnfRules/Rule.hpp>

#include <algorithm>
#include <cstddef>

/* ************************************************************************** */
// PUBLIC

AlternativeRule::AlternativeRule()
  : _mode(Greedy)
{
  setDebugTag("Alternative");
}

AlternativeRule::~AlternativeRule()
{
  for (std::size_t i = 0; i < _rules.size(); i++) {
    delete _rules[i];
  }
}

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
  setEndOfRule(false);
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

void AlternativeRule::addRule(Rule* rule)
{
  _rules.push_back(rule);
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
      setEndPos(std::max(getBuffReader()->getPosInBuff(), getEndPos()));
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
