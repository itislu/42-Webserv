#include "RepetitionRule.hpp"
#include "Rule.hpp"

#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/BufferReader.hpp>

#include <limits>

/* ************************************************************************** */
// PUBLIC
RepetitionRule::RepetitionRule(ft::unique_ptr<Rule> rule)
  : _minReps(0)
  , _maxReps(std::numeric_limits<int>::max())
  , _currReps(0)
  , _reachedMin(_currReps >= _minReps)
  , _rule(ft::move(rule))
{
  setEndOfRule(false);
  setDebugTag("Repetition");
}

RepetitionRule::~RepetitionRule() {}

bool RepetitionRule::matches()
{
  debugPrintRuleEntry();

  _rule->setDebugPrintIndent(getDebugPrintIndent() + 2);

  while (!getBuffReader()->reachedEnd()) {

    setStartPos(getBuffReader()->getPosInBuff());

    const bool matches = _rule->matches();
    _rule->reset();
    if (matches) {
      _incrementReps();
      _reachedMin = _currReps >= _minReps;

      if (_currReps >= _maxReps ||
          getStartPos() == getBuffReader()->getPosInBuff()) {
        break;
      }
    } else {
      rewindToStartPos();
      break;
    }
  }

  if (_minReps <= 0) {
    setDebugMatchReason("_minReps == 0");
    _reachedMin = true;
  }
  addRuleResult(_reachedMin);
  debugPrintMatchStatus(_reachedMin);
  return _reachedMin;
}

void RepetitionRule::reset()
{
  _reachedMin = false;
  _currReps = 0;
  setEndOfRule(false);
  _rule->reset();
}

void RepetitionRule::setBufferReader(BufferReader* bufferReader)
{
  Rule::setBufferReader(bufferReader);
  _rule->setBufferReader(bufferReader);
}

void RepetitionRule::setResultMap(ResultMap* results)
{
  Rule::setResultMap(results);
  _rule->setResultMap(results);
}

void RepetitionRule::setMin(int value)
{
  _minReps = value;
}

void RepetitionRule::setMax(int value)
{
  _maxReps = value;
}

bool RepetitionRule::reachedMin() const
{
  return _reachedMin;
}

int RepetitionRule::getReps() const
{
  return _currReps;
}

/* ************************************************************************** */
// PRIVATE

void RepetitionRule::_incrementReps()
{
  if (_currReps < std::numeric_limits<int>::max()) {
    _currReps++;
  }
}
