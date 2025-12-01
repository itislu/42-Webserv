#include "generalRules.hpp"

#include <http/abnfRules/ruleIds.hpp>
#include <http/http.hpp>
#include <libftpp/memory.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/RangeRule.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>

ft::shared_ptr<LiteralRule> endOfLineRule()
{
  const ft::shared_ptr<LiteralRule> seq =
    ft::make_shared<LiteralRule>(http::CRLF);

  seq->setDebugTag("endOfLineRule");
  seq->setRuleId(EndOfLine);
  return seq;
}

/**
 * OWS            = *( SP / HTAB )
 *                ; optional whitespace
 */
ft::shared_ptr<RepetitionRule> owsRule()
{
  const ft::shared_ptr<RepetitionRule> rep = ft::make_shared<RepetitionRule>(
    ft::make_shared<RangeRule>(http::isWhitespace));

  rep->setDebugTag("owsRule");
  return rep;
}

/**
 * RWS            = 1*( SP / HTAB )
 *                ; required whitespace
 */
ft::shared_ptr<RepetitionRule> rwsRule()
{
  const ft::shared_ptr<RepetitionRule> rep = ft::make_shared<RepetitionRule>(
    ft::make_shared<RangeRule>(http::isWhitespace));
  rep->setMin(1);

  rep->setDebugTag("rwsRule");
  return rep;
}

/**
 * BWS            = OWS
 *                ; bad whitespace
 */
ft::shared_ptr<RepetitionRule> bwsRule()
{
  const ft::shared_ptr<RepetitionRule> rep = owsRule();

  rep->setDebugTag("bwsRule");
  return rep;
}
