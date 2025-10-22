#include "AlternativRule.hpp"

#include <algorithm>
#include <utils/abnfRules/Rule.hpp>
#include <utils/BufferReader.hpp>

#include <cstddef>

/* ************************************************************************** */
// PUBLIC

AlternativRule::AlternativRule()
  : _mode(Greedy)
{
  setDebugTag("Alternativ");
}

AlternativRule::~AlternativRule()
{
  for (std::size_t i = 0; i < _rules.size(); i++) {
    delete _rules[i];
  }
}

bool AlternativRule::matches()
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
    default:
      matches = false;
  }

  addRuleResult(matches);
  debugPrintMatchStatus(matches);
  return matches;
}

void AlternativRule::reset()
{
  for (std::size_t i = 0; i < _rules.size(); i++) {
    _rules[i]->reset();
  }
  setEndOfRule(false);
}

void AlternativRule::setBufferReader(BufferReader* bufferReader)
{
  Rule::setBufferReader(bufferReader);
  for (std::size_t i = 0; i < _rules.size(); i++) {
    _rules[i]->setBufferReader(bufferReader);
  }
}

void AlternativRule::setResultMap(ResultMap* results)
{
  Rule::setResultMap(results);
  for (std::size_t i = 0; i < _rules.size(); i++) {
    _rules[i]->setResultMap(results);
  }
}

void AlternativRule::addRule(Rule* rule)
{
  _rules.push_back(rule);
}

/* ************************************************************************** */
// PRIVATE

AlternativRule::AlternativRule(const AlternativRule& other)
  : _mode(Greedy)
{
  *this = other;
}

AlternativRule& AlternativRule::operator=(const AlternativRule& other)
{
  if (this != &other) {
    // todo copy logic
  }
  return *this;
}

bool AlternativRule::_firstMatchMode()
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

bool AlternativRule::_greedyMode()
{
  setStartPos(getBuffReader()->getPosInBuff());
  setEndPos(-1);
  for (std::size_t i = 0; i < _rules.size(); i++) {

    _rules[i]->setDebugPrintIndent(getDebugPrintIndent() + 2);

    if (_rules[i]->matches()) {
      setEndPos(std::max(getBuffReader()->getPosInBuff(), getEndPos()));
    }
    rewindToStartPos();
  }
  if (getEndPos() == -1) {
    return false;
  }
  moveToEndPos();
  return true;
}
