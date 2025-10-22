#include "requestLineRules.hpp"

#include "http/abnfRules/ruleIds.hpp"
#include <http/abnfRules/uriRules.hpp>
#include <utils/abnfRules/AlternativRule.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>
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

  AlternativRule* terminator = new AlternativRule();
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

  // const char terminatorChars[] = "?# "; // todo why not working
  AlternativRule* terminator = new AlternativRule();
  terminator->addRule(new LiteralRule("?"));
  terminator->addRule(new LiteralRule("#"));
  terminator->addRule(new LiteralRule(" "));
  seq->addRule(terminator);

  seq->setDebugTag("pathPartRule");
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

SequenceRule* endOfRequestLineRule()
{
  SequenceRule* seq = new SequenceRule();
  seq->addRule(new LiteralRule("\r"));
  seq->addRule(new LiteralRule("\n"));

  seq->setDebugTag("endOfRequestLineRule");
  return seq;
}

RepetitionRule* whitespaceRule(int min, int max)
{
  RepetitionRule* rep = new RepetitionRule(new LiteralRule(" "));
  rep->setMin(min);
  rep->setMax(max);
  return rep;
}
