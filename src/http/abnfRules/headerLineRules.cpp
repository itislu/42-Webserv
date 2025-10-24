#include "http/abnfRules/generalRules.hpp"
#include "http/abnfRules/ruleIds.hpp"
#include "http/http.hpp"
#include "libftpp/memory.hpp"
#include "libftpp/utility.hpp"
#include "utils/abnfRules/LiteralRule.hpp"
#include "utils/abnfRules/RangeRule.hpp"
#include "utils/abnfRules/RepetitionRule.hpp"
#include "utils/abnfRules/SequenceRule.hpp"
#include <http/abnfRules/headerLineRules.hpp>

/**
 * field-line CRLF
 */
ft::unique_ptr<SequenceRule> fieldLinePartRule()
{
  ft::unique_ptr<SequenceRule> seq = ft::make_unique<SequenceRule>();
  seq->addRule(fieldLineRule());
  seq->addRule(endOfLineRule());

  seq->setDebugTag("fieldLinePartRule");
  seq->setRuleId(FieldLinePart);
  return ft::move(seq);
}

/**
 * field-line   = field-name ":" OWS field-value OWS
 */
SequenceRule* fieldLineRule()
{
  SequenceRule* seq = new SequenceRule();
  seq->addRule(fieldNameRule());
  seq->addRule(new LiteralRule(":"));
  seq->addRule(owsRule());
  seq->addRule(fieldValueRule());
  seq->addRule(owsRule());

  seq->setDebugTag("fieldLineRule");
  return seq;
}

// TODO this is just a test function
RepetitionRule* fieldNameRule()
{
  RepetitionRule* rep = new RepetitionRule(new RangeRule(http::isTchar));

  rep->setDebugTag("fieldNameRule");
  return rep;
}

// TODO this is just a test function
RepetitionRule* fieldValueRule()
{
  RepetitionRule* rep = new RepetitionRule(new RangeRule(http::isTchar));

  rep->setDebugTag("fieldValueRule");
  return rep;
}
