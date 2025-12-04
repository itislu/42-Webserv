#include "headerRules.hpp"

#include <http/abnfRules/generalRules.hpp>
#include <http/abnfRules/headerLineRules.hpp>
#include <http/abnfRules/http11Rules.hpp>
#include <http/abnfRules/ruleIds.hpp>
#include <http/abnfRules/uriRules.hpp>
#include <libftpp/ctype.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/abnfRules/AlternativeRule.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/RangeRule.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>
#include <utils/abnfRules/Rule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

/**
 * field-line   = field-name ":" OWS field-value OWS
 */
ft::shared_ptr<SequenceRule> fieldLineWrapper(
  ft::shared_ptr<Rule> fieldValueRule)
{
  const ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();
  seq->addRule(owsRule());
  seq->addRule(ft::move(fieldValueRule));
  seq->addRule(owsRule());
  seq->addRule(endOfLineRule());
  return seq;
}

/**
 * Host = uri-host [ ":" port ]
 */
ft::shared_ptr<SequenceRule> hostHeaderRule()
{
  const ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();

  // host
  seq->addRule(hostRule());

  // [ ":" port ]
  ft::shared_ptr<SequenceRule> portSeq = ft::make_shared<SequenceRule>();
  portSeq->addRule(ft::make_shared<LiteralRule>(":"));
  portSeq->addRule(portRule());
  portSeq->setRuleId(Port);

  ft::shared_ptr<RepetitionRule> optPort =
    ft::make_shared<RepetitionRule>(ft::move(portSeq));
  optPort->setMin(0);
  optPort->setMax(1);
  optPort->setDebugTag("optPort");

  seq->addRule(ft::move(optPort));

  seq->setDebugTag("hostHeaderRule");
  return seq;
}

/**
 * Content-Length = 1*DIGIT
 */
ft::shared_ptr<RepetitionRule> contentLengthRule()
{
  const ft::shared_ptr<RepetitionRule> rep =
    ft::make_shared<RepetitionRule>(ft::make_shared<RangeRule>(ft::isdigit));
  rep->setMin(1);
  return rep;
}

/**
 *
 * Sender:
 * Transfer-Encoding = [ transfer-coding *( OWS "," OWS transfer-coding ) ]
 *
 * Receipient:
 * #element => [ element ] *( OWS "," OWS [ element ] )
 */
ft::shared_ptr<SequenceRule> transferEncodingRule()
{
  const ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();

  seq->addRule(listRule(transferCodingRule()));

  seq->setDebugTag("transferEncodingRule");
  return seq;
}

/**
 * transfer-coding = token *( OWS ";" OWS transfer-parameter )
 */
ft::shared_ptr<SequenceRule> transferCodingRule()
{
  const ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();

  // token
  seq->addRule(tokenRule());

  // *( OWS ";" OWS transfer-parameter )
  ft::shared_ptr<SequenceRule> ext = ft::make_shared<SequenceRule>();
  ext->addRule(owsRule());
  ext->addRule(ft::make_shared<LiteralRule>(";"));
  ext->addRule(owsRule());
  ext->addRule(transferParameterRule());

  seq->addRule(ft::make_shared<RepetitionRule>(ft::move(ext)));

  seq->setDebugTag("transferCodingRule");
  return seq;
}

/**
 * transfer-parameter = token BWS "=" BWS ( token / quoted-string )
 */
ft::shared_ptr<SequenceRule> transferParameterRule()
{
  const ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();

  seq->addRule(tokenRule());
  seq->addRule(bwsRule());
  seq->addRule(ft::make_shared<LiteralRule>("="));
  seq->addRule(bwsRule());

  // ( token / quoted-string )
  ft::shared_ptr<AlternativeRule> alter = ft::make_shared<AlternativeRule>();
  alter->addRule(tokenRule());
  alter->addRule(quotedStringRule());
  seq->addRule(ft::move(alter));

  seq->setDebugTag("transferParameterRule");
  return seq;
}
