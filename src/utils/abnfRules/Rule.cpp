#include "Rule.hpp"

#include <libftpp/format.hpp>
#include <libftpp/utility.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/RuleResult.hpp>

#include <cstddef>
#include <iomanip>
#include <iostream>

/* ************************************************************************** */
// INIT

bool Rule::debugPrint = false;

/* ************************************************************************** */
// PUBLIC

Rule::Rule()
  : _reachedEnd(false)
  , _buffReader()
  , _startPos()
  , _endPos()
  , _ruleId(_ruleIdUndefined)
  , _results(FT_NULLPTR)
  , _debugTag(FT_NULLPTR)
  , _debugMatchReason(FT_NULLPTR)
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
  _reachedEnd = false;
}

void Rule::setBufferReader(BufferReader* bufferReader)
{
  _buffReader = bufferReader;
}

void Rule::setResultMap(ResultMap* results)
{
  _results = results;
}

bool Rule::reachedEnd() const
{
  return _reachedEnd;
}

void Rule::setRuleId(RuleId ruleId)
{
  _ruleId = ruleId;
}

void Rule::setDebugTag(const char* str)
{
  _debugTag = str;
}

const char* Rule::getDebugTag() const
{
  return _debugTag;
}

void Rule::setDebugMatchReason(const char* reason)
{
  _debugMatchReason = reason;
}

/* ************************************************************************** */
// PROTECTED

void Rule::setReachedEnd(bool value)
{
  _reachedEnd = value;
}

BufferReader* Rule::getBuffReader()
{
  return _buffReader;
}

void Rule::setStartPos(std::size_t pos)
{
  _startPos = pos;
}

std::size_t Rule::getStartPos() const
{
  return _startPos;
}

void Rule::setEndPos(std::size_t pos)
{
  _endPos = pos;
}

std::size_t Rule::getEndPos() const
{
  return _endPos;
}

void Rule::rewindToStartPos()
{
  const std::size_t currPos = getBuffReader()->getPosInBuff();
  const std::size_t diff = currPos - getStartPos();
  getBuffReader()->rewind(diff);
}

void Rule::moveToEndPos()
{
  getBuffReader()->setPosInBuff(getEndPos());
}

void Rule::addRuleResult(bool matches)
{
  if (_results == FT_NULLPTR || _ruleId == _ruleIdUndefined || !matches) {
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
  const int indent = 40;
  std::cout << std::setw(indent) << _debugTag << ": entry " << ">\n";
}

void Rule::debugPrintMatchStatus(bool matches)
{
  if (!debugPrint) {
    return;
  }
  const int indent = 40;
  std::cout << std::setw(indent) << _debugTag << ":       ";
  if (matches) {
    std::cout << ft::green("match   ");
  } else {
    std::cout << ft::red("no match");
  }

  if (_debugMatchReason != FT_NULLPTR) {
    std::cout << "  (" << _debugMatchReason << ")";
  }
  std::cout << "\n";
}

/* ************************************************************************** */
// PRIVATE
