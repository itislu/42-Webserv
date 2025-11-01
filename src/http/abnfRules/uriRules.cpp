#include "uriRules.hpp"

#include <http/http.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/abnfRules/AlternativeRule.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/RangeRule.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

#include <ctype.h>

/**
 * Reference:
 * https://datatracker.ietf.org/doc/html/rfc3986#autoid-69
 */

/**
 * URI = scheme ":" hier-part [ "?" query ] [ "#" fragment ]
 */
ft::shared_ptr<SequenceRule> uriRule()
{
  const ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();

  // scheme ":"
  seq->addRule(schemeRule());
  seq->addRule(ft::make_shared<LiteralRule>(":"));

  // hier-part
  seq->addRule(hierPartRule());

  // [ "?" query ]
  ft::shared_ptr<SequenceRule> querySeq = ft::make_shared<SequenceRule>();
  querySeq->addRule(ft::make_shared<LiteralRule>("?"));
  querySeq->addRule(queryRule());

  ft::shared_ptr<RepetitionRule> optQuery =
    ft::make_shared<RepetitionRule>(ft::move(querySeq));
  optQuery->setMin(0);
  optQuery->setMax(1);
  seq->addRule(ft::move(optQuery));

  // [ "#" fragment ]
  ft::shared_ptr<SequenceRule> fragSeq = ft::make_shared<SequenceRule>();
  fragSeq->addRule(ft::make_shared<LiteralRule>("#"));
  fragSeq->addRule(fragmentRule());

  ft::shared_ptr<RepetitionRule> optFrag =
    ft::make_shared<RepetitionRule>(ft::move(fragSeq));
  optFrag->setMin(0);
  optFrag->setMax(1);
  seq->addRule(ft::move(optFrag));

  seq->setDebugTag("uriRule");
  return seq;
}

/**
 * hier-part = "//" authority path-abempty
 *            / path-absolute
 *            / path-rootless
 *            / path-empty
 */
ft::shared_ptr<AlternativeRule> hierPartRule()
{
  const ft::shared_ptr<AlternativeRule> alter =
    ft::make_shared<AlternativeRule>();

  // "//" authority path-abempty
  ft::shared_ptr<SequenceRule> seq1 = ft::make_shared<SequenceRule>();
  seq1->addRule(ft::make_shared<LiteralRule>("//"));
  seq1->addRule(authorityRule());
  seq1->addRule(pathAbEmptyRule());
  alter->addRule(ft::move(seq1));

  // path-absolute
  alter->addRule(pathAbsoluteRule());

  // path-rootless
  alter->addRule(pathRootlessRule());

  // path-empty
  alter->addRule(pathEmptyRule());

  alter->setDebugTag("hierPartRule");
  return alter;
}

/**
 * scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
 */
ft::shared_ptr<SequenceRule> schemeRule()
{
  ft::shared_ptr<RangeRule> range =
    ft::make_shared<RangeRule>(http::isSchemeChar);
  ft::shared_ptr<RepetitionRule> rep =
    ft::make_shared<RepetitionRule>(ft::move(range));

  const ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();
  seq->addRule(ft::make_shared<RangeRule>(::isalpha));
  seq->addRule(ft::move(rep));

  seq->setDebugTag("schemeRule");
  return seq;
}

/**
 * authority = [ userinfo "@" ] host [ ":" port ]
 */
ft::shared_ptr<SequenceRule> authorityRule()
{
  const ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();

  // [ userinfo "@" ]
  ft::shared_ptr<SequenceRule> userinfoSeq = ft::make_shared<SequenceRule>();
  userinfoSeq->addRule(userinfoRule());
  userinfoSeq->addRule(ft::make_shared<LiteralRule>("@"));
  userinfoSeq->setDebugTag("userinfoSeq");

  ft::shared_ptr<RepetitionRule> optUserinfo =
    ft::make_shared<RepetitionRule>(ft::move(userinfoSeq));
  optUserinfo->setMin(0);
  optUserinfo->setMax(1);
  optUserinfo->setDebugTag("optUserInfo");

  seq->addRule(ft::move(optUserinfo));

  // host
  seq->addRule(hostRule());

  // [ ":" port ]
  ft::shared_ptr<SequenceRule> portSeq = ft::make_shared<SequenceRule>();
  portSeq->addRule(ft::make_shared<LiteralRule>(":"));
  portSeq->addRule(portRule());

  ft::shared_ptr<RepetitionRule> optPort =
    ft::make_shared<RepetitionRule>(ft::move(portSeq));
  optPort->setMin(0);
  optPort->setMax(1);
  optPort->setDebugTag("optPort");

  seq->addRule(ft::move(optPort));

  seq->setDebugTag("authoritySeq");
  return seq;
}

/**
 * userinfo = *( unreserved / pct-encoded / sub-delims / ":" )
 */
ft::shared_ptr<RepetitionRule> userinfoRule()
{
  ft::shared_ptr<AlternativeRule> alter = ft::make_shared<AlternativeRule>();
  alter->addRule(ft::make_shared<RangeRule>(http::isUnreserved));
  alter->addRule(pctRule());
  alter->addRule(ft::make_shared<RangeRule>(http::isSubDelim));
  alter->addRule(ft::make_shared<LiteralRule>(":"));

  const ft::shared_ptr<RepetitionRule> rep =
    ft::make_shared<RepetitionRule>(ft::move(alter));
  rep->setDebugTag("userinfoRule");
  return rep;
}

/**
 * host          = IP-literal / IPv4address / reg-name
 */
ft::shared_ptr<AlternativeRule> hostRule()
{
  const ft::shared_ptr<AlternativeRule> alter =
    ft::make_shared<AlternativeRule>();
  alter->addRule(ipLiteralRule());
  alter->addRule(ipv4AddressRule());
  alter->addRule(regNameRule());

  alter->setDebugTag("hostRule");
  return alter;
}

/**
 * port = *DIGIT
 */
ft::shared_ptr<RepetitionRule> portRule()
{
  const ft::shared_ptr<RepetitionRule> rep =
    ft::make_shared<RepetitionRule>(ft::make_shared<RangeRule>(::isdigit));
  rep->setDebugTag("portRule");
  return rep;
}

/**
 * IP-literal = "[" ( IPv6address / IPvFuture ) "]"
 */
ft::shared_ptr<SequenceRule> ipLiteralRule()
{
  const ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();

  // "["
  seq->addRule(ft::make_shared<LiteralRule>("["));

  // ( IPv6address / IPvFuture )
  ft::shared_ptr<AlternativeRule> alter = ft::make_shared<AlternativeRule>();
  alter->addRule(ipv6AddressRule());
  alter->addRule(ipvFutureRule());
  seq->addRule(ft::move(alter));

  // "]"
  seq->addRule(ft::make_shared<LiteralRule>("]"));

  seq->setDebugTag("ipLiteralRule");
  return seq;
}

/**
 * IPvFuture = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
 */
ft::shared_ptr<SequenceRule> ipvFutureRule()
{
  const ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();

  // "v"
  seq->addRule(ft::make_shared<LiteralRule>("v"));

  // 1*HEXDIG
  ft::shared_ptr<RepetitionRule> hexRep = ft::make_shared<RepetitionRule>(
    ft::make_shared<RangeRule>(http::isHexDigit));
  hexRep->setMin(1);
  seq->addRule(ft::move(hexRep));

  // "."
  seq->addRule(ft::make_shared<LiteralRule>("."));

  // 1*( unreserved / sub-delims / ":" )
  ft::shared_ptr<AlternativeRule> alter = ft::make_shared<AlternativeRule>();
  alter->addRule(ft::make_shared<RangeRule>(http::isUnreserved));
  alter->addRule(ft::make_shared<RangeRule>(http::isSubDelim));
  alter->addRule(ft::make_shared<LiteralRule>(":"));

  ft::shared_ptr<RepetitionRule> rep =
    ft::make_shared<RepetitionRule>(ft::move(alter));
  rep->setMin(1);
  seq->addRule(ft::move(rep));

  seq->setDebugTag("ipvFutureRule");
  return seq;
}

/**
 * ipv6AddressRule helper function for repeating "h16 ':'"
 */
static ft::shared_ptr<RepetitionRule> h16_colon(int min, int max)
{
  ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();
  seq->addRule(h16Rule());
  seq->addRule(ft::make_shared<LiteralRule>(":"));

  const ft::shared_ptr<RepetitionRule> rep =
    ft::make_shared<RepetitionRule>(ft::move(seq));
  rep->setMin(min);
  rep->setMax(max);
  return rep;
}

/**
 *                                          6( h16 ":" ) ls32
 */
static ft::shared_ptr<SequenceRule> ipv6AddressRuleAlt1()
{
  const int min = 6;
  const int max = 6;
  const ft::shared_ptr<SequenceRule> rule1 = ft::make_shared<SequenceRule>();
  rule1->addRule(h16_colon(min, max));
  rule1->addRule(ls32Rule());
  return rule1;
}

/**
 *             /                       "::" 5( h16 ":" ) ls32
 */
static ft::shared_ptr<SequenceRule> ipv6AddressRuleAlt2()
{
  const int min = 5;
  const int max = 5;
  const ft::shared_ptr<SequenceRule> rule2 = ft::make_shared<SequenceRule>();
  rule2->addRule(ft::make_shared<LiteralRule>("::"));
  rule2->addRule(h16_colon(min, max));
  rule2->addRule(ls32Rule());
  return rule2;
}

/**
 *             / [               h16 ] "::" 4( h16 ":" ) ls32
 */
static ft::shared_ptr<SequenceRule> ipv6AddressRuleAlt3()
{
  const ft::shared_ptr<SequenceRule> rule = ft::make_shared<SequenceRule>();
  ft::shared_ptr<RepetitionRule> opt_h16 =
    ft::make_shared<RepetitionRule>(h16Rule());
  opt_h16->setMin(0);
  opt_h16->setMax(1);
  rule->addRule(ft::move(opt_h16));
  rule->addRule(ft::make_shared<LiteralRule>("::"));
  rule->addRule(h16_colon(4, 4));
  rule->addRule(ls32Rule());
  return rule;
}

/**
 *             / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
 */
static ft::shared_ptr<SequenceRule> ipv6AddressRuleAlt4()
{
  const ft::shared_ptr<SequenceRule> rule = ft::make_shared<SequenceRule>();
  ft::shared_ptr<SequenceRule> opt = ft::make_shared<SequenceRule>();
  opt->addRule(h16_colon(0, 1));
  opt->addRule(h16Rule());
  ft::shared_ptr<RepetitionRule> optWrap =
    ft::make_shared<RepetitionRule>(ft::move(opt));
  optWrap->setMin(0);
  optWrap->setMax(1);
  rule->addRule(ft::move(optWrap));
  rule->addRule(ft::make_shared<LiteralRule>("::"));
  rule->addRule(h16_colon(3, 3));
  rule->addRule(ls32Rule());
  return rule;
}

/**
 *             / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
 */
static ft::shared_ptr<SequenceRule> ipv6AddressRuleAlt5()
{
  const ft::shared_ptr<SequenceRule> rule = ft::make_shared<SequenceRule>();
  ft::shared_ptr<SequenceRule> opt = ft::make_shared<SequenceRule>();
  opt->addRule(h16_colon(0, 2));
  opt->addRule(h16Rule());
  ft::shared_ptr<RepetitionRule> optWrap =
    ft::make_shared<RepetitionRule>(ft::move(opt));
  optWrap->setMin(0);
  optWrap->setMax(1);
  rule->addRule(ft::move(optWrap));
  rule->addRule(ft::make_shared<LiteralRule>("::"));
  rule->addRule(h16_colon(2, 2));
  rule->addRule(ls32Rule());
  return rule;
}

/**
 *             / [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
 */
static ft::shared_ptr<SequenceRule> ipv6AddressRuleAlt6()
{
  const ft::shared_ptr<SequenceRule> rule = ft::make_shared<SequenceRule>();
  ft::shared_ptr<SequenceRule> opt = ft::make_shared<SequenceRule>();
  opt->addRule(h16_colon(0, 3));
  opt->addRule(h16Rule());
  ft::shared_ptr<RepetitionRule> optWrap =
    ft::make_shared<RepetitionRule>(ft::move(opt));
  optWrap->setMin(0);
  optWrap->setMax(1);
  rule->addRule(ft::move(optWrap));
  rule->addRule(ft::make_shared<LiteralRule>("::"));
  rule->addRule(h16Rule());
  rule->addRule(ft::make_shared<LiteralRule>(":"));
  rule->addRule(ls32Rule());
  return rule;
}

/**
 *             / [ *4( h16 ":" ) h16 ] "::"              ls32
 */
static ft::shared_ptr<SequenceRule> ipv6AddressRuleAlt7()
{
  const ft::shared_ptr<SequenceRule> rule = ft::make_shared<SequenceRule>();
  ft::shared_ptr<SequenceRule> opt = ft::make_shared<SequenceRule>();
  opt->addRule(h16_colon(0, 4));
  opt->addRule(h16Rule());
  ft::shared_ptr<RepetitionRule> optWrap =
    ft::make_shared<RepetitionRule>(ft::move(opt));
  optWrap->setMin(0);
  optWrap->setMax(1);
  rule->addRule(ft::move(optWrap));
  rule->addRule(ft::make_shared<LiteralRule>("::"));
  rule->addRule(ls32Rule());
  return rule;
}

/**
 *             / [ *5( h16 ":" ) h16 ] "::"              h16
 */
static ft::shared_ptr<SequenceRule> ipv6AddressRuleAlt8()
{
  const int min = 0;
  const int max = 5;
  const ft::shared_ptr<SequenceRule> rule = ft::make_shared<SequenceRule>();
  ft::shared_ptr<SequenceRule> opt = ft::make_shared<SequenceRule>();
  opt->addRule(h16_colon(min, max));
  opt->addRule(h16Rule());
  ft::shared_ptr<RepetitionRule> optWrap =
    ft::make_shared<RepetitionRule>(ft::move(opt));
  optWrap->setMin(0);
  optWrap->setMax(1);
  rule->addRule(ft::move(optWrap));
  rule->addRule(ft::make_shared<LiteralRule>("::"));
  rule->addRule(h16Rule());
  return rule;
}

/**
 *             / [ *6( h16 ":" ) h16 ] "::"
 */
static ft::shared_ptr<SequenceRule> ipv6AddressRuleAlt9()
{
  const int min = 0;
  const int max = 6;
  const ft::shared_ptr<SequenceRule> rule = ft::make_shared<SequenceRule>();
  ft::shared_ptr<SequenceRule> opt = ft::make_shared<SequenceRule>();
  opt->addRule(h16_colon(min, max));
  opt->addRule(h16Rule());
  ft::shared_ptr<RepetitionRule> optWrap =
    ft::make_shared<RepetitionRule>(ft::move(opt));
  optWrap->setMin(0);
  optWrap->setMax(1);
  rule->addRule(ft::move(optWrap));
  rule->addRule(ft::make_shared<LiteralRule>("::"));
  return rule;
}

/**
 * IPv6address =
 *                                          6( h16 ":" ) ls32
 *             /                       "::" 5( h16 ":" ) ls32
 *             / [               h16 ] "::" 4( h16 ":" ) ls32
 *             / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
 *             / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
 *             / [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
 *             / [ *4( h16 ":" ) h16 ] "::"              ls32
 *             / [ *5( h16 ":" ) h16 ] "::"              h16
 *             / [ *6( h16 ":" ) h16 ] "::"
 */
ft::shared_ptr<AlternativeRule> ipv6AddressRule()
{
  const ft::shared_ptr<AlternativeRule> alter =
    ft::make_shared<AlternativeRule>();
  alter->addRule(ipv6AddressRuleAlt1());
  alter->addRule(ipv6AddressRuleAlt2());
  alter->addRule(ipv6AddressRuleAlt3());
  alter->addRule(ipv6AddressRuleAlt4());
  alter->addRule(ipv6AddressRuleAlt5());
  alter->addRule(ipv6AddressRuleAlt6());
  alter->addRule(ipv6AddressRuleAlt7());
  alter->addRule(ipv6AddressRuleAlt8());
  alter->addRule(ipv6AddressRuleAlt9());
  alter->setDebugTag("ipv6AddressRule");
  return alter;
}

/**
 * h16 = 1*4HEXDIG
 *       ; 16 bits of address represented in hexadecimal
 */
ft::shared_ptr<RepetitionRule> h16Rule()
{
  const ft::shared_ptr<RepetitionRule> rep = ft::make_shared<RepetitionRule>(
    ft::make_shared<RangeRule>(http::isHexDigit));
  rep->setMin(1);
  rep->setMax(4);
  rep->setDebugTag("h16Rule");
  return rep;
}

/**
 * ls32 = ( h16 ":" h16 ) / IPv4address
 *        ; least-significant 32 bits of address
 */
ft::shared_ptr<AlternativeRule> ls32Rule()
{
  const ft::shared_ptr<AlternativeRule> alter =
    ft::make_shared<AlternativeRule>();

  // h16 ":" h16
  ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();
  seq->addRule(h16Rule());
  seq->addRule(ft::make_shared<LiteralRule>(":"));
  seq->addRule(h16Rule());
  alter->addRule(ft::move(seq));

  // IPv4address
  alter->addRule(ipv4AddressRule());

  alter->setDebugTag("ls32Rule");
  return alter;
}

/**
 * IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet
 */
ft::shared_ptr<SequenceRule> ipv4AddressRule()
{
  const ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();

  seq->addRule(decOctetRule());
  seq->addRule(ft::make_shared<LiteralRule>("."));
  seq->addRule(decOctetRule());
  seq->addRule(ft::make_shared<LiteralRule>("."));
  seq->addRule(decOctetRule());
  seq->addRule(ft::make_shared<LiteralRule>("."));
  seq->addRule(decOctetRule());

  seq->setDebugTag("ipv4AddressRule");
  return seq;
}

/**
 * dec-octet = DIGIT                 ; 0-9
 *            / %x31-39 DIGIT        ; 10-99
 *            / "1" 2DIGIT           ; 100-199
 *            / "2" %x30-34 DIGIT    ; 200-249
 *            / "25" %x30-35         ; 250-255
 */
ft::shared_ptr<AlternativeRule> decOctetRule()
{
  const ft::shared_ptr<AlternativeRule> alter =
    ft::make_shared<AlternativeRule>();

  // DIGIT → 0-9
  alter->addRule(ft::make_shared<RangeRule>(::isdigit));

  // %x31-39 DIGIT → [1-9][0-9]
  ft::shared_ptr<SequenceRule> rule10_99 = ft::make_shared<SequenceRule>();
  rule10_99->addRule(ft::make_shared<RangeRule>(http::isDigit19));
  rule10_99->addRule(ft::make_shared<RangeRule>(::isdigit));
  alter->addRule(ft::move(rule10_99));

  // "1" 2DIGIT → 100–199
  ft::shared_ptr<SequenceRule> rule100_199 = ft::make_shared<SequenceRule>();
  rule100_199->addRule(ft::make_shared<LiteralRule>("1"));
  rule100_199->addRule(ft::make_shared<RangeRule>(::isdigit));
  rule100_199->addRule(ft::make_shared<RangeRule>(::isdigit));
  alter->addRule(ft::move(rule100_199));

  // "2" %x30-34 DIGIT → 200–249
  ft::shared_ptr<SequenceRule> rule200_249 = ft::make_shared<SequenceRule>();
  rule200_249->addRule(ft::make_shared<LiteralRule>("2"));
  rule200_249->addRule(ft::make_shared<RangeRule>(http::isDigit04));
  rule200_249->addRule(ft::make_shared<RangeRule>(::isdigit));
  alter->addRule(ft::move(rule200_249));

  // "25" %x30-35 → 250–255
  ft::shared_ptr<SequenceRule> rule250_255 = ft::make_shared<SequenceRule>();
  rule250_255->addRule(ft::make_shared<LiteralRule>("25"));
  rule250_255->addRule(ft::make_shared<RangeRule>(http::isDigit05));
  alter->addRule(ft::move(rule250_255));

  alter->setDebugTag("decOctetRule");
  return alter;
}

/**
 * reg-name      = *( unreserved / pct-encoded / sub-delims )
 */
ft::shared_ptr<RepetitionRule> regNameRule()
{
  ft::shared_ptr<AlternativeRule> alter = ft::make_shared<AlternativeRule>();
  alter->addRule(ft::make_shared<RangeRule>(http::isUnreserved));
  alter->addRule(pctRule());
  alter->addRule(ft::make_shared<RangeRule>(http::isSubDelim));

  const ft::shared_ptr<RepetitionRule> rep =
    ft::make_shared<RepetitionRule>(ft::move(alter));
  rep->setDebugTag("regNameRule");
  return rep;
}

/**
 *   path          = path-abempty    ; begins with "/" or is empty
 *                 / path-absolute   ; begins with "/" but not "//"
 *                 / path-noscheme   ; begins with a non-colon segment
 *                 / path-rootless   ; begins with a segment
 *                 / path-empty      ; zero characters
 *
 *   path-abempty  = *( "/" segment )
 *   path-absolute = "/" [ segment-nz *( "/" segment ) ]
 *   path-noscheme = segment-nz-nc *( "/" segment )
 *   path-rootless = segment-nz *( "/" segment )
 *   path-empty    = 0<pchar>
 */
ft::shared_ptr<AlternativeRule> pathRule()
{
  const ft::shared_ptr<AlternativeRule> alter =
    ft::make_shared<AlternativeRule>();
  alter->addRule(pathAbEmptyRule());
  alter->addRule(pathAbsoluteRule());
  alter->addRule(pathNoSchemeRule());
  alter->addRule(pathRootlessRule());
  alter->addRule(pathEmptyRule());

  alter->setDebugTag("pathRule");
  return alter;
}

/**
 * path-abempty  = *( "/" segment )
 */
ft::shared_ptr<RepetitionRule> pathAbEmptyRule()
{
  ft::shared_ptr<SequenceRule> sequence = ft::make_shared<SequenceRule>();
  sequence->addRule(ft::make_shared<LiteralRule>("/"));
  sequence->addRule(segmentRule());

  const ft::shared_ptr<RepetitionRule> rep =
    ft::make_shared<RepetitionRule>(ft::move(sequence));
  rep->setDebugTag("pathAbEmptyRule");
  return rep;
}

/**
 * path-absolute = "/" [ segment-nz *( "/" segment ) ]
 */
ft::shared_ptr<SequenceRule> pathAbsoluteRule()
{
  ft::shared_ptr<SequenceRule> optSeq = ft::make_shared<SequenceRule>();
  optSeq->addRule(segmentNzRule());
  optSeq->addRule(pathAbEmptyRule());

  ft::shared_ptr<RepetitionRule> option =
    ft::make_shared<RepetitionRule>(ft::move(optSeq));
  // to make it actually optional we set the limits between 0 and 1
  // also described here:
  // https://datatracker.ietf.org/doc/html/rfc2234#autoid-15
  option->setMin(0);
  option->setMax(1);

  const ft::shared_ptr<SequenceRule> sequence = ft::make_shared<SequenceRule>();
  sequence->addRule(ft::make_shared<LiteralRule>("/"));
  sequence->addRule(ft::move(option));

  sequence->setDebugTag("pathAbsoluteRule");
  return sequence;
}

/**
 * path-noscheme = segment-nz-nc *( "/" segment )
 */
ft::shared_ptr<SequenceRule> pathNoSchemeRule()
{
  const ft::shared_ptr<SequenceRule> sequence = ft::make_shared<SequenceRule>();
  sequence->addRule(segmentNzNcRule());
  sequence->addRule(pathAbEmptyRule());
  sequence->setDebugTag("pathNoSchemeRule");
  return sequence;
}

/**
 * path-rootless = segment-nz *( "/" segment )
 */
ft::shared_ptr<SequenceRule> pathRootlessRule()
{
  const ft::shared_ptr<SequenceRule> sequence = ft::make_shared<SequenceRule>();
  sequence->addRule(segmentNzRule());
  sequence->addRule(pathAbEmptyRule());

  sequence->setDebugTag("pathRootlessRule");
  return sequence;
}

/**
 * path-empty    = 0<pchar>
 */
ft::shared_ptr<RepetitionRule> pathEmptyRule()
{
  const ft::shared_ptr<RepetitionRule> rep =
    ft::make_shared<RepetitionRule>(pcharRule());
  rep->setMin(0);
  rep->setMax(0);

  rep->setDebugTag("pathEmptyRule");
  return rep;
}

/**
 * segment      = *pchar
 */
ft::shared_ptr<RepetitionRule> segmentRule()
{
  const ft::shared_ptr<RepetitionRule> rep =
    ft::make_shared<RepetitionRule>(pcharRule());
  rep->setDebugTag("segmentRule");
  return rep;
}

/**
 * segment-nz    = 1*pchar
 */
ft::shared_ptr<RepetitionRule> segmentNzRule()
{
  const ft::shared_ptr<RepetitionRule> rep =
    ft::make_shared<RepetitionRule>(pcharRule());
  rep->setMin(1);
  rep->setDebugTag("segmentNzRule");
  return rep;
}

/**
 * segment-nz-nc = 1*( unreserved / pct-encoded / sub-delims / "@" )
 *                 ; non-zero-length segment without any colon ":"
 */
ft::shared_ptr<RepetitionRule> segmentNzNcRule()
{
  ft::shared_ptr<AlternativeRule> alter = ft::make_shared<AlternativeRule>();
  alter->addRule(ft::make_shared<RangeRule>(http::isUnreserved));
  alter->addRule(pctRule());
  alter->addRule(ft::make_shared<RangeRule>(http::isSubDelim));
  alter->addRule(ft::make_shared<LiteralRule>("@"));

  const ft::shared_ptr<RepetitionRule> rep =
    ft::make_shared<RepetitionRule>(ft::move(alter));
  rep->setMin(1);
  rep->setDebugTag("segmentNzNcRule");
  return rep;
}

/**
 * pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
 */
ft::shared_ptr<AlternativeRule> pcharRule()
{
  const ft::shared_ptr<AlternativeRule> alter =
    ft::make_shared<AlternativeRule>();
  alter->addRule(ft::make_shared<RangeRule>(http::isPchar));
  alter->addRule(pctRule());

  alter->setDebugTag("pcharRule");
  return alter;
}

/**
 * query = *( pchar / "/" / "?" )
 */
ft::shared_ptr<RepetitionRule> queryRule()
{
  ft::shared_ptr<AlternativeRule> alter = ft::make_shared<AlternativeRule>();
  alter->addRule(pcharRule());
  alter->addRule(ft::make_shared<RangeRule>(http::isFragmentChar));

  const ft::shared_ptr<RepetitionRule> rep =
    ft::make_shared<RepetitionRule>(ft::move(alter));
  rep->setDebugTag("queryRule");
  return rep;
}

/**
 * fragment = *( pchar / "/" / "?" )
 */
ft::shared_ptr<RepetitionRule> fragmentRule()
{
  ft::shared_ptr<AlternativeRule> alter = ft::make_shared<AlternativeRule>();
  alter->addRule(pcharRule());
  alter->addRule(ft::make_shared<RangeRule>(http::isFragmentChar));

  const ft::shared_ptr<RepetitionRule> rep =
    ft::make_shared<RepetitionRule>(ft::move(alter));
  rep->setDebugTag("fragmentRule");
  return rep;
}

/**
 * pct-encoded   = "%" HEXDIG HEXDIG
 */
ft::shared_ptr<SequenceRule> pctRule()
{
  const ft::shared_ptr<SequenceRule> pctSeq = ft::make_shared<SequenceRule>();
  pctSeq->addRule(ft::make_shared<LiteralRule>("%"));
  pctSeq->addRule(ft::make_shared<RangeRule>(http::isHexDigit));
  pctSeq->addRule(ft::make_shared<RangeRule>(http::isHexDigit));

  pctSeq->setDebugTag("pctRule");
  return pctSeq;
}
