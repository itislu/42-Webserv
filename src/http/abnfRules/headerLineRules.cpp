#include <http/abnfRules/generalRules.hpp>
#include <http/abnfRules/headerLineRules.hpp>
#include <http/abnfRules/ruleIds.hpp>
#include <http/http.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/abnfRules/AlternativeRule.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/RangeRule.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

#include <cctype>

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

/**
 * field-name = token
 * token = 1*tchar
 */
RepetitionRule* fieldNameRule()
{
  RepetitionRule* rep = tokenRule();

  rep->setDebugTag("fieldNameRule");
  return rep;
}

RepetitionRule* tokenRule()
{
  RepetitionRule* rep = new RepetitionRule(new RangeRule(http::isTchar));
  rep->setMin(1);
  rep->setDebugTag("tokenRule");
  return rep;
}

/**
 * field-value = *field-content
 */
RepetitionRule* fieldValueRule()
{
  RepetitionRule* rep = new RepetitionRule(fieldContentRule());
  rep->setDebugTag("fieldValueRule");
  return rep;
}

/**
 * field-content = field-vchar
 *                 [ 1*( SP / HTAB / field-vchar ) field-vchar ]
 */
SequenceRule* fieldContentRule()
{
  SequenceRule* seq = new SequenceRule();

  // leading field-vchar
  seq->addRule(fieldVcharRule());

  // 1*( SP / HTAB / field-vchar )
  AlternativeRule* spaceTabVchar = new AlternativeRule();
  spaceTabVchar->addRule(new RangeRule(" \t"));  // SP, HTAB
  spaceTabVchar->addRule(fieldVcharRule());
  RepetitionRule* repeatPart = new RepetitionRule(spaceTabVchar);
  repeatPart->setMin(1);

  SequenceRule* optSeq = new SequenceRule();
  optSeq->addRule(repeatPart);
  optSeq->addRule(fieldVcharRule());

  RepetitionRule* optWrap = new RepetitionRule(optSeq);
  optWrap->setMin(0);
  optWrap->setMax(1);

  seq->addRule(optWrap);
  seq->setDebugTag("fieldContentRule");
  return seq;
}

/**
 * field-vchar = VCHAR / obs-text
 */
AlternativeRule* fieldVcharRule()
{
  AlternativeRule* alter = new AlternativeRule();
  alter->addRule(new RangeRule(http::isVchar));
  alter->addRule(obsTextRule());
  alter->setDebugTag("fieldVcharRule");
  return alter;
}

/**
 * https://datatracker.ietf.org/doc/html/rfc9110/#name-field-values
 *
 * obs-text = %x80-FF
 */
RangeRule* obsTextRule()
{
  RangeRule* range = new RangeRule(http::isObsText);
  range->setDebugTag("obsTextRule");
  return range;
}
