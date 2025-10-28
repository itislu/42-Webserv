#include "generalRules.hpp"

#include <http/abnfRules/ruleIds.hpp>
#include <http/http.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/RangeRule.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

SequenceRule* endOfLineRule()
{
  SequenceRule* seq = new SequenceRule();
  seq->addRule(new LiteralRule("\r"));
  seq->addRule(new LiteralRule("\n"));

  seq->setDebugTag("endOfLineRule");
  seq->setRuleId(EndOfLine);
  return seq;
}

/**
 * OWS            = *( SP / HTAB )
 *                ; optional whitespace
 */
RepetitionRule* owsRule()
{
  RepetitionRule* rep = new RepetitionRule(new RangeRule(http::isWhitespace));

  rep->setDebugTag("owsRule");
  return rep;
}

/**
 * RWS            = 1*( SP / HTAB )
 * ; required whitespace
 */
RepetitionRule* rwsRule()
{
  RepetitionRule* rep = new RepetitionRule(new RangeRule(http::isWhitespace));
  rep->setMin(1);

  rep->setDebugTag("owsRule");
  return rep;
}
