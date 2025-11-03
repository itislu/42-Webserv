#include "requestLineRules.hpp"

#include <http/abnfRules/ruleIds.hpp>
#include <http/abnfRules/uriRules.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/RangeRule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

/**
 * authority-part = "//" authority
 *                  ; terminated by "/" or "?" or "#" or end of URI
 */
ft::unique_ptr<SequenceRule> authorityPartRule()
{
  ft::unique_ptr<SequenceRule> seq = ft::make_unique<SequenceRule>();

  seq->addRule(ft::make_shared<LiteralRule>("//"));
  seq->addRule(authorityRule());

  ft::shared_ptr<RangeRule> terminator = ft::make_shared<RangeRule>("/?# ");
  seq->addRule(ft::move(terminator));

  seq->setDebugTag("authorityPartRule");
  seq->setRuleId(AuthorityPart);
  return ft::move(seq);
}

/**
 * The path is terminated
 * by the first question mark ("?") or number sign ("#") character, or
 * by the end of the URI.
 */
ft::unique_ptr<SequenceRule> pathPartRule()
{
  ft::unique_ptr<SequenceRule> seq = ft::make_unique<SequenceRule>();

  seq->addRule(pathRule());

  ft::shared_ptr<RangeRule> terminator = ft::make_shared<RangeRule>("?# ");
  seq->addRule(ft::move(terminator));

  seq->setDebugTag("pathPartRule");
  seq->setRuleId(PathPart);
  return ft::move(seq);
}

ft::unique_ptr<SequenceRule> schemePartRule()
{
  ft::unique_ptr<SequenceRule> seq = ft::make_unique<SequenceRule>();

  seq->addRule(schemeRule());
  seq->addRule(ft::make_shared<LiteralRule>(":"));
  seq->setRuleId(SchemePart);

  seq->setDebugTag("schemePartRule");
  return ft::move(seq);
}

ft::unique_ptr<SequenceRule> queryPartRule()
{
  ft::unique_ptr<SequenceRule> seq = ft::make_unique<SequenceRule>();

  seq->addRule(ft::make_shared<LiteralRule>("?"));
  seq->addRule(queryRule());
  ft::shared_ptr<RangeRule> terminator = ft::make_shared<RangeRule>("# ");
  seq->addRule(ft::move(terminator));

  seq->setDebugTag("queryPartRule");
  seq->setRuleId(QueryPart);
  return ft::move(seq);
}

ft::unique_ptr<SequenceRule> fragmentPartRule()
{
  ft::unique_ptr<SequenceRule> seq = ft::make_unique<SequenceRule>();

  seq->addRule(ft::make_shared<LiteralRule>("#"));
  seq->addRule(fragmentRule());

  ft::shared_ptr<LiteralRule> terminator = ft::make_shared<LiteralRule>(" ");
  seq->addRule(ft::move(terminator));

  seq->setDebugTag("fragmentPartRule");
  seq->setRuleId(FragmentPart);
  return ft::move(seq);
}
