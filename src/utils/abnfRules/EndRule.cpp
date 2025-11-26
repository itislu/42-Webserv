#include "EndRule.hpp"
#include "Rule.hpp"

#include <cstddef>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/BufferReader.hpp>

/* ************************************************************************** */
// PUBLIC
EndRule::EndRule(ft::shared_ptr<Rule> rule)
  : _rule(ft::move(rule))
  , _pos(0)
{
}

/**
 * @brief Go back char by char and check if the buffer ends with this rule
 */
bool EndRule::matches()
{
  debugPrintRuleEntry();
  setEndPos(getBuffReader()->getPosInBuff());
  bool matches = false;
  std::size_t rewindCount = 1;
  while (!matches && getBuffReader()->getPosInBuff() > getStartPos() &&
         getBuffReader()->getPosInBuff() > 0) {
    getBuffReader()->rewind(rewindCount);
    matches = _rule->matches();
    if (matches && getEndPos() == getBuffReader()->getPosInBuff()) {
      break;
    }
    moveToEndPos();
    rewindCount++;
    matches = false;
  }
  moveToEndPos();
  setReachedEnd(matches);
  debugPrintMatchStatus(matches);
  return matches;
}

void EndRule::reset()
{
  setReachedEnd(false);
  _rule->reset();
}

void EndRule::setBufferReader(BufferReader* bufferReader)
{
  Rule::setBufferReader(bufferReader);
  _rule->setBufferReader(bufferReader);
}

void EndRule::setResultMap(ResultMap* results)
{
  Rule::setResultMap(results);
  _rule->setResultMap(results);
}
