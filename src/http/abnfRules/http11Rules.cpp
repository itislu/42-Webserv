#include "http11Rules.hpp"

#include <http/abnfRules/ruleIds.hpp>
#include <http/abnfRules/uriRules.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/abnfRules/AlternativeRule.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

// https://datatracker.ietf.org/doc/html/rfc9112#name-collected-abnf

/**
 * request-target = origin-form
 *                  / absolute-form
 *                  / authority-form
 *                  / asterisk-form
 */
ft::shared_ptr<AlternativeRule> requestTargetRule()
{
  const ft::shared_ptr<AlternativeRule> alter =
    ft::make_shared<AlternativeRule>();
  alter->addRule(originFormRule());
  alter->addRule(absoluteFormRule());
  // alter->addRule(authorityFormRule()); // not supported
  // alter->addRule(asteriskFormRule());  // not supported

  alter->setDebugTag("origin-form / absolute-form");
  return alter;
}

/**
 * origin-form = absolute-path [ "?" query ]
 */
ft::shared_ptr<SequenceRule> originFormRule()
{
  // Optional part: "?" query
  ft::shared_ptr<SequenceRule> optSeq = ft::make_shared<SequenceRule>();
  optSeq->addRule(ft::make_shared<LiteralRule>("?"));
  optSeq->addRule(queryRule());

  ft::shared_ptr<RepetitionRule> optional =
    ft::make_shared<RepetitionRule>(ft::move(optSeq));
  optional->setMin(0);
  optional->setMax(1);

  // Main sequence: absolute-path [ "?" query ]
  const ft::shared_ptr<SequenceRule> sequence = ft::make_shared<SequenceRule>();
  sequence->addRule(absolutePathRule());
  sequence->addRule(ft::move(optional));

  sequence->setDebugTag("originFormRule");
  sequence->setRuleId(OriginForm);
  return sequence;
}

/**
 * absolute-path = 1*( "/" segment )
 */
ft::shared_ptr<RepetitionRule> absolutePathRule()
{
  // "/" segment
  ft::shared_ptr<SequenceRule> sequence = ft::make_shared<SequenceRule>();
  sequence->addRule(ft::make_shared<LiteralRule>("/"));
  sequence->addRule(segmentRule());

  // 1*( "/" segment )
  const ft::shared_ptr<RepetitionRule> rep =
    ft::make_shared<RepetitionRule>(ft::move(sequence));
  rep->setMin(1);

  rep->setRuleId(AbsolutePath);
  return rep;
}

/**
 * absolute-form = absolute-URI
 *
 * Special parts of the uri will be handled later.
 * For example the host can be empty by the ABNF but the RFC says:
 * A sender MUST NOT generate an "http" URI with an empty host identifier.
 */
ft::shared_ptr<SequenceRule> absoluteFormRule()
{
  const ft::shared_ptr<SequenceRule> seq = absoluteUriRule();
  seq->setRuleId(AbsoluteForm);
  return seq;
}
