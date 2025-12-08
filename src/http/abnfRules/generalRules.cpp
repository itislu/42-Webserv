#include "generalRules.hpp"

#include <http/abnfRules/ruleIds.hpp>
#include <http/http.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/RangeRule.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>
#include <utils/abnfRules/Rule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

/**
 * https://datatracker.ietf.org/doc/html/rfc5234#appendix-B.1
 *
 * CRLF           =  CR LF
 *                ; Internet standard newline
 */
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
  const ft::shared_ptr<RepetitionRule> rep =
    ft::make_shared<RepetitionRule>(ft::make_shared<RangeRule>(" \t"));

  rep->setDebugTag("owsRule");
  return rep;
}

/**
 * RWS            = 1*( SP / HTAB )
 *                ; required whitespace
 */
ft::shared_ptr<RepetitionRule> rwsRule()
{
  const ft::shared_ptr<RepetitionRule> rep =
    ft::make_shared<RepetitionRule>(ft::make_shared<RangeRule>(" \t"));
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

/**
 * #element => [ element ] *( OWS "," OWS [ element ] )
 */
ft::shared_ptr<SequenceRule> listRule(ft::shared_ptr<Rule> elementRule)
{
  // ---------- optional leading element ----------
  ft::shared_ptr<RepetitionRule> leadingOpt =
    ft::make_shared<RepetitionRule>(elementRule);
  leadingOpt->setMin(0); // optional
  leadingOpt->setMax(1);

  // ---------- single comma-group: OWS "," OWS [ element ] ----------
  ft::shared_ptr<SequenceRule> onePart = ft::make_shared<SequenceRule>();
  onePart->addRule(owsRule());
  onePart->addRule(ft::make_shared<LiteralRule>(",")); // comma
  onePart->addRule(owsRule());

  // optional element after comma
  ft::shared_ptr<RepetitionRule> optElement =
    ft::make_shared<RepetitionRule>(ft::move(elementRule));
  optElement->setMin(0);
  optElement->setMax(1);

  onePart->addRule(ft::move(optElement));

  // ---------- repetition of comma-groups ----------
  ft::shared_ptr<RepetitionRule> repeating =
    ft::make_shared<RepetitionRule>(ft::move(onePart));

  // ---------- full sequence ----------
  const ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();
  seq->addRule(ft::move(leadingOpt));
  seq->addRule(ft::move(repeating));

  seq->setDebugTag("listRule");
  return seq;
}
