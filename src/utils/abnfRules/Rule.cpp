#include "Rule.hpp"

#include <libftpp/format.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/RuleResult.hpp>

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <string>

/* ************************************************************************** */
// INIT

bool Rule::debugPrint = false;

/* ************************************************************************** */
// PUBLIC

Rule::Rule()
  : _endOfRule(false)
  , _buffReader()
  , _startPos()
  , _endPos()
  , _ruleId(_ruleIdUndefined)
  , _results(NULL)
  , _debugPrintIndent(_debugInitPrintIndent)
  , _debugTag(NULL)
  , _debugMatchReason(NULL)
{
  setDebugTag("Base Rule");
}

Rule::~Rule() {}

bool Rule::matches()
{
  return false;
}

void Rule::reset()
{
  _endOfRule = false;
}

void Rule::setBufferReader(BufferReader* bufferReader)
{
  _buffReader = bufferReader;
}

void Rule::setResultMap(ResultMap* results)
{
  _results = results;
}

bool Rule::end() const
{
  return _endOfRule;
}

void Rule::setRuleId(RuleId ruleId)
{
  _ruleId = ruleId;
}

void Rule::setDebugTag(const char* str)
{
  _debugTag = str;
}

void Rule::setDebugPrintIndent(int value)
{
  _debugPrintIndent = value;
}

int Rule::getDebugPrintIndent() const
{
  return _debugPrintIndent;
}

void Rule::setDebugMatchReason(const char* reason)
{
  _debugMatchReason = reason;
}

/* ************************************************************************** */
// PROTECTED

void Rule::setEndOfRule(bool value)
{
  _endOfRule = value;
}

BufferReader* Rule::getBuffReader()
{
  return _buffReader;
}

void Rule::setStartPos(long pos)
{
  _startPos = pos;
}

long Rule::getStartPos() const
{
  return _startPos;
}

void Rule::setEndPos(long pos)
{
  _endPos = pos;
}

long Rule::getEndPos() const
{
  return _endPos;
}

void Rule::rewindToStartPos()
{
  const long currPos = getBuffReader()->getPosInBuff();
  const long diff = currPos - getStartPos();
  getBuffReader()->rewind(diff);
}

void Rule::moveToEndPos()
{
  getBuffReader()->setPosInBuff(getEndPos());
}

void Rule::addRuleResult(bool matches)
{
  if (_results == NULL || _ruleId == _ruleIdUndefined || !matches) {
    return;
  }
  ResultMap& results = *_results;
  results[_ruleId] = RuleResult(_startPos, _endPos);
}

void Rule::debugPrintRuleEntry()
{
  if (!debugPrint) {
    return;
  }
  const int indent = 20;
  _debugPrintIndent += 2;
  std::cout << std::setw(indent) << _debugTag << ": entry "
            << std::string(_debugPrintIndent, '-') << ">\n";
}

void Rule::debugPrintMatchStatus(bool matches)
{
  if (!debugPrint) {
    return;
  }
  const int indent = 20;
  std::cout << std::setw(indent) << _debugTag << ":       ";
  if (matches) {
    std::cout << std::string(_debugPrintIndent, ' ');
    std::cout << ft::green("match");
  } else {
    std::cout << std::string(_debugPrintIndent, ' ');
    std::cout << ft::red("no match");
  }

  if (_debugMatchReason != NULL) {
    std::cout << "  (" << _debugMatchReason << ")";
  }

  std::cout << "   '";
  _buffReader->printRemaining();
  std::cout << "'\n";
}

/* ************************************************************************** */
// PRIVATE
