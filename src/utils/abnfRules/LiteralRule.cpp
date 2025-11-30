#include "LiteralRule.hpp"

#include <utils/abnfRules/Rule.hpp>
#include <utils/buffer/IInBuffer.hpp>

/* ************************************************************************** */
// PUBLIC

LiteralRule::LiteralRule(const char* literal)
  : _pos(0)
  , _literal(literal)
{
  setDebugTag("Literal");
}

LiteralRule::~LiteralRule() {}

bool LiteralRule::matches()
{
  setStartPos(getBuffReader()->pos());
  debugPrintRuleEntry();
  bool matches = true;
  while (!bufferReachedEnd()) {
    const char chr = getBuffReader()->get();
    matches = chr == _literal[_pos];
    _pos++;
    if (_pos >= _literal.size()) {
      setReachedEnd(true);
      break;
    }
    if (!matches) {
      break;
    }
  }
  setEndPos(getBuffReader()->pos());
  addRuleResult(matches);
  debugPrintMatchStatus(matches);
  return matches;
}

void LiteralRule::reset()
{
  _pos = 0;
  setReachedEnd(false);
}

void LiteralRule::setBufferReader(IInBuffer* bufferReader)
{
  Rule::setBufferReader(bufferReader);
}

void LiteralRule::setResultMap(ResultMap* results)
{
  Rule::setResultMap(results);
}

/* ************************************************************************** */
// PRIVATE
