#include "LiteralRule.hpp"

#include <utils/BufferReader.hpp>
#include <utils/abnfRules/Rule.hpp>

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
  setStartPos(getBuffReader()->getPosInBuff());
  debugPrintRuleEntry();
  bool matches = true;
  while (!getBuffReader()->reachedEnd()) {
    const char chr = getBuffReader()->getNextChar();
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
  setEndPos(getBuffReader()->getPosInBuff());
  addRuleResult(matches);
  debugPrintMatchStatus(matches);
  return matches;
}

void LiteralRule::reset()
{
  _pos = 0;
  setReachedEnd(false);
}

void LiteralRule::setBufferReader(BufferReader* bufferReader)
{
  Rule::setBufferReader(bufferReader);
}

void LiteralRule::setResultMap(ResultMap* results)
{
  Rule::setResultMap(results);
}

/* ************************************************************************** */
// PRIVATE
