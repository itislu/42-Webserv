#include "headerLineRules.hpp"

#include <http/abnfRules/generalRules.hpp>
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
ft::shared_ptr<SequenceRule> fieldLineRule()
{
  const ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();
  seq->addRule(fieldNameRule());
  seq->addRule(ft::make_shared<LiteralRule>(":"));
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
ft::shared_ptr<RepetitionRule> fieldNameRule()
{
  const ft::shared_ptr<RepetitionRule> rep = tokenRule();

  rep->setDebugTag("fieldNameRule");
  return rep;
}

ft::shared_ptr<RepetitionRule> tokenRule()
{
  const ft::shared_ptr<RepetitionRule> rep =
    ft::make_shared<RepetitionRule>(ft::make_shared<RangeRule>(http::isTchar));
  rep->setMin(1);
  rep->setDebugTag("tokenRule");
  return rep;
}

/**
 * field-value = *field-content
 */
ft::shared_ptr<RepetitionRule> fieldValueRule()
{
  const ft::shared_ptr<RepetitionRule> rep =
    ft::make_shared<RepetitionRule>(fieldContentRule());
  rep->setDebugTag("fieldValueRule");
  return rep;
}

/**
 * field-content = field-vchar
 *                 [ 1*( SP / HTAB / field-vchar ) field-vchar ]
 */
ft::shared_ptr<SequenceRule> fieldContentRule()
{
  const ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();

  // leading field-vchar
  seq->addRule(fieldVcharRule());

  // 1*( SP / HTAB / field-vchar )
  ft::shared_ptr<AlternativeRule> spaceTabVchar =
    ft::make_shared<AlternativeRule>();
  spaceTabVchar->addRule(ft::make_shared<RangeRule>(" \t")); // SP, HTAB
  spaceTabVchar->addRule(fieldVcharRule());
  spaceTabVchar->setDebugTag("SP/HTAB/field-vchar");

  ft::shared_ptr<RepetitionRule> repeatPart =
    ft::make_shared<RepetitionRule>(ft::move(spaceTabVchar));
  repeatPart->setDebugTag("1*(SP/HTAB/field-vchar)");
  repeatPart->setMin(1);

  ft::shared_ptr<SequenceRule> optSeq = ft::make_shared<SequenceRule>();
  optSeq->setDebugTag("[1*(SP/HTAB/field-vchar)field-vchar]");
  optSeq->addRule(ft::move(repeatPart));
  // optSeq->addRule(fieldVcharRule()); // todo issue #58

  ft::shared_ptr<RepetitionRule> optWrap =
    ft::make_shared<RepetitionRule>(ft::move(optSeq));
  optWrap->setMin(0);
  optWrap->setMax(1);

  seq->addRule(ft::move(optWrap));
  seq->setDebugTag("fieldContentRule");
  return seq;
}

/**
 * field-vchar = VCHAR / obs-text
 */
ft::shared_ptr<AlternativeRule> fieldVcharRule()
{
  const ft::shared_ptr<AlternativeRule> alter =
    ft::make_shared<AlternativeRule>();
  alter->addRule(ft::make_shared<RangeRule>(http::isVchar));
  alter->addRule(obsTextRule());
  alter->setDebugTag("VCHAR/obs-text");
  return alter;
}

/**
 * https://datatracker.ietf.org/doc/html/rfc9110/#name-field-values
 *
 * obs-text = %x80-FF
 */
ft::shared_ptr<RangeRule> obsTextRule()
{
  const ft::shared_ptr<RangeRule> range =
    ft::make_shared<RangeRule>(http::isObsText);
  range->setDebugTag("obsText");
  return range;
}
