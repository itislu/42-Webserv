#include "RangeRule.hpp"

#include <libftpp/utility.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/Rule.hpp>

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

RangeRule::~RangeRule() {}

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

  switch (_range.type) {
    case Func:
      matches = _range.func(chr) != 0;
      break;
    case Charset:
      matches = std::strchr(_range.set, chr) != FT_NULLPTR;
      break;
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
