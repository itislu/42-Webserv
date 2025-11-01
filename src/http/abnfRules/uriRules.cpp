#include "uriRules.hpp"
#include "libftpp/memory.hpp"
#include "libftpp/utility.hpp"
#include "utils/abnfRules/Rule.hpp"

#include <http/http.hpp>
#include <memory>
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
// ft::unique_ptr<SequenceRule> uriRule()
// {
//   ft::unique_ptr<SequenceRule> seq = new SequenceRule();

//   // scheme ":"
//   seq->addRule(schemeRule());
//   seq->addRule(new LiteralRule(":"));

//   // hier-part
//   seq->addRule(hierPartRule());

//   // [ "?" query ]
//   ft::unique_ptr<SequenceRule> querySeq = new SequenceRule();
//   querySeq->addRule(new LiteralRule("?"));
//   querySeq->addRule(queryRule());

//   RepetitionRule* optQuery = new RepetitionRule(querySeq);
//   optQuery->setMin(0);
//   optQuery->setMax(1);
//   seq->addRule(optQuery);

//   // [ "#" fragment ]
//   ft::unique_ptr<SequenceRule> fragSeq = new SequenceRule();
//   fragSeq->addRule(new LiteralRule("#"));
//   fragSeq->addRule(fragmentRule());

//   RepetitionRule* optFrag = new RepetitionRule(fragSeq);
//   optFrag->setMin(0);
//   optFrag->setMax(1);
//   seq->addRule(optFrag);

//   seq->setDebugTag("uriRule");
//   return seq;
// }

/**
 * hier-part = "//" authority path-abempty
 *            / path-absolute
 *            / path-rootless
 *            / path-empty
 */
// AlternativeRule* hierPartRule()
// {
//   AlternativeRule* alter = new AlternativeRule();

//   // "//" authority path-abempty
//   ft::unique_ptr<SequenceRule> seq1 = new SequenceRule();
//   seq1->addRule(new LiteralRule("//"));
//   seq1->addRule(authorityRule());
//   seq1->addRule(pathAbEmptyRule());
//   alter->addRule(seq1);

//   // path-absolute
//   alter->addRule(pathAbsoluteRule());

//   // path-rootless
//   alter->addRule(pathRootlessRule());

//   // path-empty
//   alter->addRule(pathEmptyRule());

//   alter->setDebugTag("hierPartRule");
//   return alter;
// }

/**
 * scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
 */
ft::unique_ptr<SequenceRule> schemeRule()
{
  // RangeRule* const range = new RangeRule(http::isSchemeChar);
  ft::unique_ptr<RangeRule> range =
    ft::make_unique<RangeRule>(http::isSchemeChar);
  ft::unique_ptr<RepetitionRule> rep =
    ft::make_unique<RepetitionRule>(ft::move(range));

  ft::unique_ptr<SequenceRule> seq = ft::make_unique<SequenceRule>();
  ft::unique_ptr<Rule> init = ft::make_unique<RangeRule>(::isalpha);
  ft::unique_ptr<Rule> init2(new RangeRule(::isalpha));
  ft::unique_ptr<Rule> init3(ft::make_unique<RangeRule>(::isalpha));
  ft::unique_ptr<int> init4(ft::make_unique<RangeRule>(::isalpha)); // ughh...
  seq->addRule(static_cast<ft::unique_ptr<Rule> >(ft::make_unique<RangeRule>(::isalpha)));
  seq->addRule(init3);
  seq->addRule(ft::move(rep));
  seq->addRule(ft::move<ft::unique_ptr<Rule> >(rep));
  seq->addRule(rep);
  seq->addRule(ft::unique_ptr<Rule>(new RangeRule(::isalpha)));
  seq->addRule(ft::unique_ptr<Rule>(ft::make_unique<RangeRule>(::isalpha)));
  seq->addRule(ft::unique_ptr<Rule>(ft::move(rep)));
  seq->addRule(rep.release());
  ft::unique_ptr<Rule> r = rep.release();
  seq->addRule(ft::make_unique<RangeRule>(::isalpha));

  
  seq->addRule(ft::unique_ptr<Rule>(ft::make_unique<RangeRule>(::isalpha)));
  seq->addRule(ft::unique_ptr<Rule>(ft::move(rep)));

  seq->setDebugTag("schemeRule");
  return ft::move(seq);
}

/**
 * authority = [ userinfo "@" ] host [ ":" port ]
 */
ft::unique_ptr<SequenceRule> authorityRule()
{
  ft::unique_ptr<SequenceRule> seq = new SequenceRule();

  // [ userinfo "@" ]
  ft::unique_ptr<SequenceRule> userinfoSeq = new SequenceRule();
  userinfoSeq->addRule(userinfoRule());
  userinfoSeq->addRule(new LiteralRule("@"));
  userinfoSeq->setDebugTag("userinfoSeq");

  RepetitionRule* optUserinfo = new RepetitionRule(userinfoSeq);
  optUserinfo->setMin(0);
  optUserinfo->setMax(1);
  optUserinfo->setDebugTag("optUserInfo");

  seq->addRule(optUserinfo);

  // host
  seq->addRule(hostRule());

  // [ ":" port ]
  ft::unique_ptr<SequenceRule> portSeq = new SequenceRule();
  portSeq->addRule(new LiteralRule(":"));
  portSeq->addRule(portRule());

  RepetitionRule* optPort = new RepetitionRule(portSeq);
  optPort->setMin(0);
  optPort->setMax(1);
  optPort->setDebugTag("optPort");

  seq->addRule(optPort);

  seq->setDebugTag("authoritySeq");
  return seq;
}

/**
 * userinfo = *( unreserved / pct-encoded / sub-delims / ":" )
 */
RepetitionRule* userinfoRule()
{
  AlternativeRule* alter = new AlternativeRule();
  alter->addRule(new RangeRule(http::isUnreserved));
  alter->addRule(pctRule());
  alter->addRule(new RangeRule(http::isSubDelim));
  alter->addRule(new LiteralRule(":"));

  RepetitionRule* rep = new RepetitionRule(alter);
  rep->setDebugTag("userinfoRule");
  return rep;
}

/**
 * host          = IP-literal / IPv4address / reg-name
 */
AlternativeRule* hostRule()
{
  AlternativeRule* alter = new AlternativeRule();
  alter->addRule(ipLiteralRule());
  alter->addRule(ipv4AddressRule());
  alter->addRule(regNameRule());

  alter->setDebugTag("hostRule");
  return alter;
}

/**
 * port = *DIGIT
 */
RepetitionRule* portRule()
{
  RepetitionRule* rep = new RepetitionRule(new RangeRule(::isdigit));
  rep->setDebugTag("portRule");
  return rep;
}

/**
 * IP-literal = "[" ( IPv6address / IPvFuture ) "]"
 */
ft::unique_ptr<SequenceRule> ipLiteralRule()
{
  ft::unique_ptr<SequenceRule> seq = new SequenceRule();

  // "["
  seq->addRule(new LiteralRule("["));

  // ( IPv6address / IPvFuture )
  AlternativeRule* alter = new AlternativeRule();
  alter->addRule(ipv6AddressRule());
  alter->addRule(ipvFutureRule());
  seq->addRule(alter);

  // "]"
  seq->addRule(new LiteralRule("]"));

  seq->setDebugTag("ipLiteralRule");
  return seq;
}

/**
 * IPvFuture = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
 */
ft::unique_ptr<SequenceRule> ipvFutureRule()
{
  ft::unique_ptr<SequenceRule> seq = new SequenceRule();

  // "v"
  seq->addRule(new LiteralRule("v"));

  // 1*HEXDIG
  RepetitionRule* hexRep = new RepetitionRule(new RangeRule(http::isHexDigit));
  hexRep->setMin(1);
  seq->addRule(hexRep);

  // "."
  seq->addRule(new LiteralRule("."));

  // 1*( unreserved / sub-delims / ":" )
  AlternativeRule* alter = new AlternativeRule();
  alter->addRule(new RangeRule(http::isUnreserved));
  alter->addRule(new RangeRule(http::isSubDelim));
  alter->addRule(new LiteralRule(":"));

  RepetitionRule* rep = new RepetitionRule(alter);
  rep->setMin(1);
  seq->addRule(rep);

  seq->setDebugTag("ipvFutureRule");
  return seq;
}

/**
 * ipv6AddressRule helper function for repeating "h16 ':'"
 */
static RepetitionRule* h16_colon(int min, int max)
{
  ft::unique_ptr<SequenceRule> seq = new SequenceRule();
  seq->addRule(h16Rule());
  seq->addRule(new LiteralRule(":"));

  RepetitionRule* rep = new RepetitionRule(seq);
  rep->setMin(min);
  rep->setMax(max);
  return rep;
}

/**
 *                                          6( h16 ":" ) ls32
 */
static ft::unique_ptr<SequenceRule> ipv6AddressRuleAlt1()
{
  const int min = 6;
  const int max = 6;
  ft::unique_ptr<SequenceRule> rule1 = new SequenceRule();
  rule1->addRule(h16_colon(min, max));
  rule1->addRule(ls32Rule());
  return rule1;
}

/**
 *             /                       "::" 5( h16 ":" ) ls32
 */
static ft::unique_ptr<SequenceRule> ipv6AddressRuleAlt2()
{
  const int min = 5;
  const int max = 5;
  ft::unique_ptr<SequenceRule> rule2 = new SequenceRule();
  rule2->addRule(new LiteralRule("::"));
  rule2->addRule(h16_colon(min, max));
  rule2->addRule(ls32Rule());
  return rule2;
}

/**
 *             / [               h16 ] "::" 4( h16 ":" ) ls32
 */
static ft::unique_ptr<SequenceRule> ipv6AddressRuleAlt3()
{
  ft::unique_ptr<SequenceRule> rule = new SequenceRule();
  RepetitionRule* opt_h16 = new RepetitionRule(h16Rule());
  opt_h16->setMin(0);
  opt_h16->setMax(1);
  rule->addRule(opt_h16);
  rule->addRule(new LiteralRule("::"));
  rule->addRule(h16_colon(4, 4));
  rule->addRule(ls32Rule());
  return rule;
}

/**
 *             / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
 */
static ft::unique_ptr<SequenceRule> ipv6AddressRuleAlt4()
{
  ft::unique_ptr<SequenceRule> rule = new SequenceRule();
  ft::unique_ptr<SequenceRule> opt = new SequenceRule();
  opt->addRule(h16_colon(0, 1));
  opt->addRule(h16Rule());
  RepetitionRule* optWrap = new RepetitionRule(opt);
  optWrap->setMin(0);
  optWrap->setMax(1);
  rule->addRule(optWrap);
  rule->addRule(new LiteralRule("::"));
  rule->addRule(h16_colon(3, 3));
  rule->addRule(ls32Rule());
  return rule;
}

/**
 *             / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
 */
static ft::unique_ptr<SequenceRule> ipv6AddressRuleAlt5()
{
  ft::unique_ptr<SequenceRule> rule = new SequenceRule();
  ft::unique_ptr<SequenceRule> opt = new SequenceRule();
  opt->addRule(h16_colon(0, 2));
  opt->addRule(h16Rule());
  RepetitionRule* optWrap = new RepetitionRule(opt);
  optWrap->setMin(0);
  optWrap->setMax(1);
  rule->addRule(optWrap);
  rule->addRule(new LiteralRule("::"));
  rule->addRule(h16_colon(2, 2));
  rule->addRule(ls32Rule());
  return rule;
}

/**
 *             / [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
 */
static ft::unique_ptr<SequenceRule> ipv6AddressRuleAlt6()
{
  ft::unique_ptr<SequenceRule> rule = new SequenceRule();
  ft::unique_ptr<SequenceRule> opt = new SequenceRule();
  opt->addRule(h16_colon(0, 3));
  opt->addRule(h16Rule());
  RepetitionRule* optWrap = new RepetitionRule(opt);
  optWrap->setMin(0);
  optWrap->setMax(1);
  rule->addRule(optWrap);
  rule->addRule(new LiteralRule("::"));
  rule->addRule(h16Rule());
  rule->addRule(new LiteralRule(":"));
  rule->addRule(ls32Rule());
  return rule;
}

/**
 *             / [ *4( h16 ":" ) h16 ] "::"              ls32
 */
static ft::unique_ptr<SequenceRule> ipv6AddressRuleAlt7()
{
  ft::unique_ptr<SequenceRule> rule = new SequenceRule();
  ft::unique_ptr<SequenceRule> opt = new SequenceRule();
  opt->addRule(h16_colon(0, 4));
  opt->addRule(h16Rule());
  RepetitionRule* optWrap = new RepetitionRule(opt);
  optWrap->setMin(0);
  optWrap->setMax(1);
  rule->addRule(optWrap);
  rule->addRule(new LiteralRule("::"));
  rule->addRule(ls32Rule());
  return rule;
}

/**
 *             / [ *5( h16 ":" ) h16 ] "::"              h16
 */
static ft::unique_ptr<SequenceRule> ipv6AddressRuleAlt8()
{
  const int min = 0;
  const int max = 5;
  ft::unique_ptr<SequenceRule> rule = new SequenceRule();
  ft::unique_ptr<SequenceRule> opt = new SequenceRule();
  opt->addRule(h16_colon(min, max));
  opt->addRule(h16Rule());
  RepetitionRule* optWrap = new RepetitionRule(opt);
  optWrap->setMin(0);
  optWrap->setMax(1);
  rule->addRule(optWrap);
  rule->addRule(new LiteralRule("::"));
  rule->addRule(h16Rule());
  return rule;
}

/**
 *             / [ *6( h16 ":" ) h16 ] "::"
 */
static ft::unique_ptr<SequenceRule> ipv6AddressRuleAlt9()
{
  const int min = 0;
  const int max = 6;
  ft::unique_ptr<SequenceRule> rule = new SequenceRule();
  ft::unique_ptr<SequenceRule> opt = new SequenceRule();
  opt->addRule(h16_colon(min, max));
  opt->addRule(h16Rule());
  RepetitionRule* optWrap = new RepetitionRule(opt);
  optWrap->setMin(0);
  optWrap->setMax(1);
  rule->addRule(optWrap);
  rule->addRule(new LiteralRule("::"));
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
AlternativeRule* ipv6AddressRule()
{
  AlternativeRule* alter = new AlternativeRule();
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
RepetitionRule* h16Rule()
{
  RepetitionRule* rep = new RepetitionRule(new RangeRule(http::isHexDigit));
  rep->setMin(1);
  rep->setMax(4);
  rep->setDebugTag("h16Rule");
  return rep;
}

/**
 * ls32 = ( h16 ":" h16 ) / IPv4address
 *        ; least-significant 32 bits of address
 */
AlternativeRule* ls32Rule()
{
  AlternativeRule* alter = new AlternativeRule();

  // h16 ":" h16
  ft::unique_ptr<SequenceRule> seq = new SequenceRule();
  seq->addRule(h16Rule());
  seq->addRule(new LiteralRule(":"));
  seq->addRule(h16Rule());
  alter->addRule(seq);

  // IPv4address
  alter->addRule(ipv4AddressRule());

  alter->setDebugTag("ls32Rule");
  return alter;
}

/**
 * IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet
 */
ft::unique_ptr<SequenceRule> ipv4AddressRule()
{
  ft::unique_ptr<SequenceRule> seq = new SequenceRule();

  seq->addRule(decOctetRule());
  seq->addRule(new LiteralRule("."));
  seq->addRule(decOctetRule());
  seq->addRule(new LiteralRule("."));
  seq->addRule(decOctetRule());
  seq->addRule(new LiteralRule("."));
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
AlternativeRule* decOctetRule()
{
  AlternativeRule* alter = new AlternativeRule();

  // DIGIT → 0-9
  alter->addRule(new RangeRule(::isdigit));

  // %x31-39 DIGIT → [1-9][0-9]
  ft::unique_ptr<SequenceRule> rule10_99 = new SequenceRule();
  rule10_99->addRule(new RangeRule(http::isDigit19));
  rule10_99->addRule(new RangeRule(::isdigit));
  alter->addRule(rule10_99);

  // "1" 2DIGIT → 100–199
  ft::unique_ptr<SequenceRule> rule100_199 = new SequenceRule();
  rule100_199->addRule(new LiteralRule("1"));
  rule100_199->addRule(new RangeRule(::isdigit));
  rule100_199->addRule(new RangeRule(::isdigit));
  alter->addRule(rule100_199);

  // "2" %x30-34 DIGIT → 200–249
  ft::unique_ptr<SequenceRule> rule200_249 = new SequenceRule();
  rule200_249->addRule(new LiteralRule("2"));
  rule200_249->addRule(new RangeRule(http::isDigit04));
  rule200_249->addRule(new RangeRule(::isdigit));
  alter->addRule(rule200_249);

  // "25" %x30-35 → 250–255
  ft::unique_ptr<SequenceRule> rule250_255 = new SequenceRule();
  rule250_255->addRule(new LiteralRule("25"));
  rule250_255->addRule(new RangeRule(http::isDigit05));
  alter->addRule(rule250_255);

  alter->setDebugTag("decOctetRule");
  return alter;
}

/**
 * reg-name      = *( unreserved / pct-encoded / sub-delims )
 */
ft::unique_ptr<RepetitionRule> regNameRule()
{
  ft::unique_ptr<AlternativeRule> alter = new AlternativeRule();
  alter->addRule(new RangeRule(http::isUnreserved));
  alter->addRule(pctRule());
  alter->addRule(new RangeRule(http::isSubDelim));

  ft::unique_ptr<RepetitionRule> rep = new RepetitionRule(alter);
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
ft::unique_ptr<AlternativeRule> pathRule()
{
  ft::unique_ptr<AlternativeRule> alter = new AlternativeRule();
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
ft::unique_ptr<RepetitionRule> pathAbEmptyRule()
{
  ft::unique_ptr<SequenceRule> sequence = new SequenceRule();
  sequence->addRule(new LiteralRule("/"));
  sequence->addRule(segmentRule());

  ft::unique_ptr<RepetitionRule> rep = new RepetitionRule(sequence);
  rep->setDebugTag("pathAbEmptyRule");
  return rep;
}

/**
 * path-absolute = "/" [ segment-nz *( "/" segment ) ]
 */
ft::unique_ptr<SequenceRule> pathAbsoluteRule()
{
  ft::unique_ptr<SequenceRule> optSeq = new SequenceRule();
  optSeq->addRule(segmentNzRule());
  optSeq->addRule(pathAbEmptyRule());

  ft::unique_ptr<RepetitionRule> option = new RepetitionRule(optSeq);
  // to make it actually optional we set the limits between 0 and 1
  // also described here:
  // https://datatracker.ietf.org/doc/html/rfc2234#autoid-15
  option->setMin(0);
  option->setMax(1);

  ft::unique_ptr<SequenceRule> sequence = new SequenceRule();
  sequence->addRule(new LiteralRule("/"));
  sequence->addRule(option);

  sequence->setDebugTag("pathAbsoluteRule");
  return sequence;
}

/**
 * path-noscheme = segment-nz-nc *( "/" segment )
 */
ft::unique_ptr<SequenceRule> pathNoSchemeRule()
{
  ft::unique_ptr<SequenceRule> sequence = new SequenceRule();
  sequence->addRule(segmentNzNcRule());
  sequence->addRule(pathAbEmptyRule());
  sequence->setDebugTag("pathNoSchemeRule");
  return sequence;
}

/**
 * path-rootless = segment-nz *( "/" segment )
 */
ft::unique_ptr<SequenceRule> pathRootlessRule()
{
  ft::unique_ptr<SequenceRule> sequence = new SequenceRule();
  sequence->addRule(segmentNzRule());
  sequence->addRule(pathAbEmptyRule());

  sequence->setDebugTag("pathRootlessRule");
  return sequence;
}

/**
 * path-empty    = 0<pchar>
 */
ft::unique_ptr<RepetitionRule> pathEmptyRule()
{
  ft::unique_ptr<RepetitionRule> rep = new RepetitionRule(pcharRule());
  rep->setMin(0);
  rep->setMax(0);

  rep->setDebugTag("pathEmptyRule");
  return rep;
}

/**
 * segment      = *pchar
 */
ft::unique_ptr<RepetitionRule> segmentRule()
{
  ft::unique_ptr<RepetitionRule> rep = new RepetitionRule(pcharRule());
  rep->setDebugTag("segmentRule");
  return rep;
}

/**
 * segment-nz    = 1*pchar
 */
ft::unique_ptr<RepetitionRule> segmentNzRule()
{
  ft::unique_ptr<RepetitionRule> rep = new RepetitionRule(pcharRule());
  rep->setMin(1);
  rep->setDebugTag("segmentNzRule");
  return rep;
}

/**
 * segment-nz-nc = 1*( unreserved / pct-encoded / sub-delims / "@" )
 *                 ; non-zero-length segment without any colon ":"
 */
ft::unique_ptr<RepetitionRule> segmentNzNcRule()
{
  ft::unique_ptr<AlternativeRule> alter = new AlternativeRule();
  alter->addRule(new RangeRule(http::isUnreserved));
  alter->addRule(pctRule());
  alter->addRule(new RangeRule(http::isSubDelim));
  alter->addRule(new LiteralRule("@"));

  RepetitionRule* rep = new RepetitionRule(alter);
  rep->setMin(1);
  rep->setDebugTag("segmentNzNcRule");
  return rep;
}

/**
 * pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
 */
ft::unique_ptr<AlternativeRule> pcharRule()
{
  ft::unique_ptr<AlternativeRule> alter = new AlternativeRule();
  alter->addRule(new RangeRule(http::isPchar));
  alter->addRule(pctRule());

  alter->setDebugTag("pcharRule");
  return alter;
}

/**
 * query = *( pchar / "/" / "?" )
 */
ft::unique_ptr<RepetitionRule> queryRule()
{
  AlternativeRule* alter = new AlternativeRule();
  alter->addRule(pcharRule());
  alter->addRule(new RangeRule(http::isFragmentChar));

  RepetitionRule* rep = new RepetitionRule(alter);
  rep->setDebugTag("queryRule");
  return rep;
}

/**
 * fragment = *( pchar / "/" / "?" )
 */
ft::unique_ptr<RepetitionRule> fragmentRule()
{
  ft::unique_ptr<AlternativeRule> alter = ft::make_unique<AlternativeRule>();
  alter->addRule(pcharRule());
  alter->addRule(new RangeRule(http::isFragmentChar));

  RepetitionRule* rep = new RepetitionRule(alter);
  rep->setDebugTag("fragmentRule");
  return ft::move(rep);
}

/**
 * pct-encoded   = "%" HEXDIG HEXDIG
 */
ft::unique_ptr<SequenceRule> pctRule()
{
  ft::unique_ptr<SequenceRule> pctSeq = ft::make_unique<SequenceRule>();
  pctSeq->addRule(ft::make_unique<LiteralRule>("%"));
  pctSeq->addRule(ft::make_nuique<RangeRule>(http::isHexDigit));
  pctSeq->addRule(ft::make_nuique<RangeRule>(http::isHexDigit));

  pctSeq->setDebugTag("pctRule");
  return ft::move(pctSeq);
}
