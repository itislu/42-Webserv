#include "requestLineRules.hpp"

#include <http/abnfRules/ruleIds.hpp>
#include <http/abnfRules/uriRules.hpp>
#include <utils/abnfRules/AlternativeRule.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/RangeRule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

/**
 * authority-part = "//" authority
 *                  ; terminated by "/" or "?" or "#" or end of URI
 */
SequenceRule* authorityPartRule()
{
  SequenceRule* seq = new SequenceRule();

  seq->addRule(new LiteralRule("//"));
  seq->addRule(authorityRule());

  AlternativeRule* terminator = new AlternativeRule();
  terminator->addRule(new LiteralRule("/"));
  terminator->addRule(new LiteralRule("?"));
  terminator->addRule(new LiteralRule("#"));
  terminator->addRule(new LiteralRule(" "));
  terminator->setDebugTag("authTerminator");
  seq->addRule(terminator);

  seq->setDebugTag("authorityPartRule");
  seq->setRuleId(AuthorityPart);
  return seq;
}

/**
 * The path is terminated
 * by the first question mark ("?") or number sign ("#") character, or
 * by the end of the URI.
 */
SequenceRule* pathPartRule()
{
  SequenceRule* seq = new SequenceRule();

  seq->addRule(pathRule());

  RangeRule* const terminator = new RangeRule("?# ");
  seq->addRule(terminator);

  seq->setDebugTag("pathPartRule");
  seq->setRuleId(PathPart);
  return seq;
}

SequenceRule* schemePartRule()
{
  SequenceRule* seq = new SequenceRule();

  seq->addRule(schemeRule());
  seq->addRule(new LiteralRule(":"));
  seq->setRuleId(SchemePart);

  seq->setDebugTag("schemePartRule");
  return seq;
}

SequenceRule* queryPartRule()
{
  SequenceRule* seq = new SequenceRule();

  seq->addRule(new LiteralRule("?"));
  seq->addRule(queryRule());
  RangeRule* const terminator = new RangeRule("# ");
  seq->addRule(terminator);

  seq->setDebugTag("queryPartRule");
  seq->setRuleId(QueryPart);
  return seq;
}

SequenceRule* fragmentPartRule()
{
  SequenceRule* seq = new SequenceRule();

  seq->addRule(new LiteralRule("#"));
  seq->addRule(fragmentRule());

  LiteralRule* const terminator = new LiteralRule(" ");
  seq->addRule(terminator);

  seq->setDebugTag("fragmentPartRule");
  seq->setRuleId(FragmentPart);
  return seq;
}
