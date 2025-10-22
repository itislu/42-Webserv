#include "LiteralRule.hpp"

#include <utils/abnfRules/Rule.hpp>
#include <utils/BufferReader.hpp>

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
      setEndOfRule(true);
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
  setEndOfRule(false);
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

LiteralRule::LiteralRule()
  : _pos(0)
{
}

LiteralRule::LiteralRule(const LiteralRule& other)
  : _pos(0)
{
  *this = other;
}

LiteralRule& LiteralRule::operator=(const LiteralRule& other)
{
  if (this != &other) {
    // todo copy logic
  }
  return *this;
}
