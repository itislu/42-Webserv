#include "RangeRule.hpp"

#include <utils/abnfRules/Rule.hpp>
#include <utils/BufferReader.hpp>

#include <cstring>

/* ************************************************************************** */
// PUBLIC

// NOLINTBEGIN(cppcoreguidelines-pro-type-union-access)
RangeRule::RangeRule(const char* range)
  : _range()
{
  _range.set = range;
  _range.type = Charset;
  setDebugTag("Range");
}
// NOLINTEND(cppcoreguidelines-pro-type-union-access)

// NOLINTBEGIN(cppcoreguidelines-pro-type-union-access)
RangeRule::RangeRule(int (*func)(int))
  : _range()
{
  _range.func = func;
  _range.type = Func;
  setDebugTag("Range func");
}
// NOLINTEND(cppcoreguidelines-pro-type-union-access)

// NOLINTBEGIN(cppcoreguidelines-pro-type-union-access)
bool RangeRule::matches()
{
  debugPrintRuleEntry();
  if (getBuffReader()->reachedEnd()) {
    return true;
  }
  const char chr = getBuffReader()->getNextChar();

  setEndOfRule(true);
  bool matches = false;
  if (_range.type == Func) {
    matches = _range.func(chr) != 0;
  } else if (_range.type == Charset) {
    matches = std::strchr(_range.set, chr) != NULL;
  }
  debugPrintMatchStatus(matches);
  return matches;
}
// NOLINTEND(cppcoreguidelines-pro-type-union-access)

void RangeRule::reset()
{
  setEndOfRule(false);
}

void RangeRule::setBufferReader(BufferReader* bufferReader)
{
  Rule::setBufferReader(bufferReader);
}

void RangeRule::setResultMap(ResultMap* results)
{
  Rule::setResultMap(results);
}

/* ************************************************************************** */
// PRIVATE

RangeRule::RangeRule()
  : _range()
{
}

RangeRule::~RangeRule() {}

RangeRule::RangeRule(const RangeRule& other)
  : _range()
{
  *this = other;
}

RangeRule& RangeRule::operator=(const RangeRule& other)
{
  if (this != &other) {
    // todo copy logic
  }
  return *this;
}
