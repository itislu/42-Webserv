#include "http11Rules.hpp"

#include <http/abnfRules/generalRules.hpp>
#include <http/abnfRules/headerLineRules.hpp>
#include <http/abnfRules/ruleIds.hpp>
#include <http/abnfRules/uriRules.hpp>
#include <http/http.hpp>
#include <libftpp/ctype.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/abnfRules/AlternativeRule.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/RangeRule.hpp>
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

/**
 * chunk          = chunk-size [ chunk-ext ] CRLF
 *                  chunk-data CRLF
 *
 * ! only first part
 */
ft::shared_ptr<SequenceRule> chunkInfoRule()
{
  const ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();
  seq->addRule(chunkSizeRule());
  seq->addRule(chunkExtRule());
  seq->addRule(endOfLineRule());

  seq->setRuleId(ChunkInfo);
  return seq;
}

/**
 * chunk-size     = 1*HEXDIG
 */
ft::shared_ptr<RepetitionRule> chunkSizeRule()
{
  const ft::shared_ptr<RepetitionRule> size =
    ft::make_shared<RepetitionRule>(ft::make_shared<RangeRule>(ft::isxdigit));
  size->setMin(1);

  size->setRuleId(ChunkSize);
  return size;
}

/**
 * last-chunk     = 1*("0") [ chunk-ext ] CRLF
 */
ft::shared_ptr<SequenceRule> lastChunkRule()
{
  ft::shared_ptr<RepetitionRule> sizeRule =
    ft::make_shared<RepetitionRule>(ft::make_shared<LiteralRule>("0"));
  sizeRule->setMin(1);
  sizeRule->setRuleId(ChunkSize);

  const ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();
  seq->addRule(ft::move(sizeRule));
  seq->addRule(chunkExtRule());
  seq->addRule(endOfLineRule());
  return seq;
}

/**
 * chunk-ext = *( BWS ";" BWS chunk-ext-name
 *                [ BWS "=" BWS chunk-ext-val ] )
 */
ft::shared_ptr<RepetitionRule> chunkExtRule()
{
  // --- Sequence for one chunk extension ---
  ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();

  // BWS ";" BWS chunk-ext-name
  seq->addRule(bwsRule());
  seq->addRule(ft::make_shared<LiteralRule>(";"));
  seq->addRule(bwsRule());
  seq->addRule(chunkExtNameRule());

  // [ BWS "=" BWS chunk-ext-val ]
  ft::shared_ptr<SequenceRule> optSeq = ft::make_shared<SequenceRule>();
  optSeq->addRule(bwsRule());
  optSeq->addRule(ft::make_shared<LiteralRule>("="));
  optSeq->addRule(bwsRule());
  optSeq->addRule(chunkExtValRule());

  ft::shared_ptr<RepetitionRule> option =
    ft::make_shared<RepetitionRule>(ft::move(optSeq));
  option->setMin(0);
  option->setMax(1);

  seq->addRule(ft::move(option));

  // Add the full sequence to the repetition
  const ft::shared_ptr<RepetitionRule> rep =
    ft::make_shared<RepetitionRule>(ft::move(seq));

  rep->setDebugTag("chunkExtRule");
  rep->setRuleId(ChunkExt);
  return rep;
}

/**
 * chunk-ext-name = token
 */
ft::shared_ptr<RepetitionRule> chunkExtNameRule()
{
  return tokenRule();
}

/**
 * chunk-ext-val  = token / quoted-string
 */
ft::shared_ptr<AlternativeRule> chunkExtValRule()
{
  const ft::shared_ptr<AlternativeRule> alter =
    ft::make_shared<AlternativeRule>();
  alter->addRule(tokenRule());
  alter->addRule(quotedStringRule());

  alter->setDebugTag("chunkExtValRule");
  return alter;
}

/**
 * qdtext = HTAB / SP / %x21 / %x23-5B / %x5D-7E / obs-text
 */
ft::shared_ptr<RangeRule> qdtextRule()
{
  const ft::shared_ptr<RangeRule> alter =
    ft::make_shared<RangeRule>(http::isQdTextChar);

  alter->setDebugTag("qdtextRule");
  return alter;
}

/**
 * quoted-pair = "\" ( HTAB / SP / VCHAR / obs-text )
 */
ft::shared_ptr<SequenceRule> quotedPairRule()
{
  const ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();
  seq->addRule(ft::make_shared<LiteralRule>("\\"));

  ft::shared_ptr<RangeRule> range =
    ft::make_shared<RangeRule>(http::isQuotedPairChar);

  seq->addRule(ft::move(range));
  seq->setDebugTag("quotedPairRule");
  return seq;
}

/**
 * quoted-string = DQUOTE *( qdtext / quoted-pair ) DQUOTE
 */
ft::shared_ptr<SequenceRule> quotedStringRule()
{
  const ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();

  // Leading DQUOTE
  seq->addRule(ft::make_shared<LiteralRule>("\""));

  // *( qdtext / quoted-pair )
  ft::shared_ptr<AlternativeRule> alter = ft::make_shared<AlternativeRule>();
  alter->addRule(qdtextRule());
  alter->addRule(quotedPairRule());

  const ft::shared_ptr<RepetitionRule> rep =
    ft::make_shared<RepetitionRule>(ft::move(alter));

  seq->addRule(rep);

  // Trailing DQUOTE
  seq->addRule(ft::make_shared<LiteralRule>("\""));

  seq->setDebugTag("quotedStringRule");
  return seq;
}
