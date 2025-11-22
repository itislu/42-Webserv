#include "utils/SmartBuffer.hpp"
#include <http/abnfRules/ruleIds.hpp>
#include <http/abnfRules/uriRules.hpp>
#include <http/http.hpp>
#include <libftpp/memory.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/AlternativeRule.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
#include <utils/abnfRules/RangeRule.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>
#include <utils/abnfRules/Rule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

#include <gtest/gtest.h>
#include <string>

/**
 * Reference:
 * https://datatracker.ietf.org/doc/html/rfc3986#autoid-69
 */

namespace {

bool runParser(const std::string& str, Rule& rule)
{
  SmartBuffer buffer;
  buffer.append(str);
  BufferReader reader(buffer);
  rule.setBufferReader(&reader);
  rule.reset();
  bool matches = rule.matches();
  if (!reader.reachedEnd()) {
    matches = false;
  }
  return matches;
}

}

// NOLINTBEGIN

/**
 * URI = scheme ":" hier-part [ "?" query ] [ "#" fragment ]
 */
TEST(UriAbnfTest, URI)
{
  ft::shared_ptr<SequenceRule> s = uriRule();
  s->addRule(ft::make_shared<RangeRule>(" "));

  // --- minimal valid schemes ---
  EXPECT_TRUE(runParser("http: ", *s));
  EXPECT_TRUE(runParser("mailto:user@example.com ", *s));
  EXPECT_TRUE(runParser("file:/home/user/test.txt ", *s));

  // --- common web URLs ---
  EXPECT_TRUE(runParser("http://example.com ", *s));
  EXPECT_TRUE(runParser("https://example.com/ ", *s));
  EXPECT_TRUE(runParser("https://example.com:443/path ", *s));
  EXPECT_TRUE(runParser("https://user:pw@host.net:8080/index.html ", *s));
  EXPECT_TRUE(runParser("ftp://ftp.example.org/resource.txt ", *s));

  // --- with query ---
  EXPECT_TRUE(runParser("https://example.com/search?q=test ", *s));
  EXPECT_TRUE(runParser("http://example.com/path/to/page?key=value&x=1 ", *s));

  // --- with fragment ---
  EXPECT_TRUE(runParser("https://example.com/index.html#section1 ", *s));
  EXPECT_TRUE(runParser("https://example.com/path?q=val#frag ", *s));

  // --- mixed case scheme (legal, case-insensitive) ---
  EXPECT_TRUE(runParser("HtTp://Example.COM ", *s));

  // --- IPv6 and IPv4 hosts ---
  EXPECT_TRUE(runParser("http://[2001:db8::1]/index.html ", *s));
  EXPECT_TRUE(runParser("http://127.0.0.1:8080/ ", *s));

  // --- encoded characters ---
  EXPECT_TRUE(runParser("https://example.com/a%20b/c%2Fd ", *s));
  EXPECT_TRUE(runParser("https://user%40mail.com@host.org ", *s));

  // frag only
  EXPECT_TRUE(runParser("http:#frag ", *s));
}

/**
 * hier-part = "//" authority path-abempty
 *            / path-absolute
 *            / path-rootless
 *            / path-empty
 */
TEST(UriAbnfTest, HierPart)
{
  // TODO if needed
}

/**
 * URI-reference = URI / relative-ref
 */
TEST(UriAbnfTest, URI_reference)
{
  // TODO if needed
}

/**
 * absolute-URI = scheme ":" hier-part [ "?" query ]
 */
TEST(UriAbnfTest, AbsoluteURI)
{
  // TODO if needed
}

/**
 * relative-ref = relative-part [ "?" query ] [ "#" fragment ]
 */
TEST(UriAbnfTest, RelativeRef)
{
  // TODO if needed
}

/**
 * relative-part = "//" authority path-abempty
 *               / path-absolute
 *               / path-noscheme
 *               / path-empty
 */
TEST(UriAbnfTest, RelativePart)
{
  // TODO if needed
}

/**
 * scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
 */
TEST(UriAbnfTest, Scheme)
{
  ft::shared_ptr<SequenceRule> sequence = schemeRule();
  EXPECT_TRUE(runParser("a1234", *sequence));
  EXPECT_TRUE(runParser("a", *sequence));
  EXPECT_TRUE(runParser("a+", *sequence));
  EXPECT_TRUE(runParser("a-", *sequence));
  EXPECT_TRUE(runParser("a.", *sequence));

  // Invalid
  EXPECT_FALSE(runParser("1a1234", *sequence));
}

/**
 * authority = [ userinfo "@" ] host [ ":" port ]
 */
TEST(UriAbnfTest, Authority)
{
  ft::shared_ptr<SequenceRule> sequence = authorityRule();
  // --- host only ---
  EXPECT_TRUE(runParser("example.com", *sequence));
  EXPECT_TRUE(runParser("localhost", *sequence));
  EXPECT_TRUE(runParser("192.168.0.1", *sequence));
  EXPECT_TRUE(runParser("[2001:db8::1]", *sequence));
  EXPECT_TRUE(runParser("[v1.fe80]", *sequence));
  EXPECT_TRUE(runParser("sub.domain.co.uk", *sequence));

  // --- host + port ---
  EXPECT_TRUE(runParser("example.com:80", *sequence));
  EXPECT_TRUE(runParser("localhost:8080", *sequence));
  EXPECT_TRUE(runParser("192.168.0.1:65535", *sequence));
  EXPECT_TRUE(runParser("[2001:db8::1]:443", *sequence));
  EXPECT_TRUE(runParser("[v1.fe80]:443", *sequence));

  // --- userinfo + host ---
  EXPECT_TRUE(runParser("@example.com", *sequence));
  EXPECT_TRUE(runParser("user@example.com", *sequence));
  EXPECT_TRUE(runParser("user:pass@example.com", *sequence));
  EXPECT_TRUE(runParser("admin@127.0.0.1", *sequence));
  EXPECT_TRUE(runParser("root@[::1]", *sequence));
  EXPECT_TRUE(runParser("root@[v4.2]", *sequence));

  // --- userinfo + host + port ---
  EXPECT_TRUE(runParser("user@example.com:80", *sequence));
  EXPECT_TRUE(runParser("user:pw@host.net:8080", *sequence));
  EXPECT_TRUE(runParser("me@[2001:db8::2]:443", *sequence));
  EXPECT_TRUE(runParser("me@[vFa.:]:443", *sequence));

  // Other
  EXPECT_TRUE(runParser("user@", *sequence));
  EXPECT_TRUE(runParser("user@:80", *sequence));
  EXPECT_TRUE(runParser("user@host:99999", *sequence));

  // Invalid
  EXPECT_FALSE(runParser("example.com: ", *sequence));
  EXPECT_FALSE(runParser("user@host:a", *sequence));
  EXPECT_FALSE(runParser("user@host:12:34", *sequence));
}

/**
 * userinfo = *( unreserved / pct-encoded / sub-delims / ":" )
 */
TEST(UriAbnfTest, UserInfo)
{
  ft::shared_ptr<RepetitionRule> rep = userinfoRule();
  EXPECT_TRUE(runParser("a1234", *rep));
  EXPECT_TRUE(runParser("a:", *rep));
}

/**
 * host = IP-literal / IPv4address / reg-name
 */
TEST(UriAbnfTest, Host)
{
  ft::shared_ptr<AlternativeRule> alter = hostRule();
  EXPECT_FALSE(runParser("example.com:abc", *alter));
}

/**
 * port = *DIGIT
 */
TEST(UriAbnfTest, Port)
{
  ft::shared_ptr<RepetitionRule> rep = portRule();
  EXPECT_FALSE(runParser("abc", *rep));
  EXPECT_TRUE(runParser("8080", *rep));
}

/**
 * IP-literal    = "[" ( IPv6address / IPvFuture  ) "]"
 */
TEST(UriAbnfTest, IP_literal)
{
  ft::shared_ptr<SequenceRule> sequence = ipLiteralRule();

  // IPv6address
  EXPECT_TRUE(runParser("[::1]", *sequence));
  EXPECT_TRUE(runParser("[2001:db8::1]", *sequence));
  EXPECT_TRUE(runParser("[::ffff:192.0.2.128]", *sequence));
  EXPECT_TRUE(runParser("[::]", *sequence));

  // IPvFuture
  EXPECT_TRUE(runParser("[v1.fe80]", *sequence));
  EXPECT_TRUE(runParser("[vF.aBc123-._~!$&'()*+,;=:0XyZ]", *sequence));

  // --- Invalid ---
  // Too many brackets.
  EXPECT_FALSE(runParser("[v1.fe80]]", *sequence));
  EXPECT_FALSE(runParser("[[v1.fe80]", *sequence));
  EXPECT_FALSE(runParser("[[v1.fe80]]", *sequence));

  // Chars outside brackets.
  EXPECT_FALSE(runParser("1[v1.fe80]", *sequence));
  EXPECT_FALSE(runParser("[v1.fe80]1", *sequence));

  // 'IPv6address / IPvFuture' not finished.
  EXPECT_FALSE(runParser("[2001:db8:1]", *sequence));
  EXPECT_FALSE(runParser("[1:2:3:4:5:6:7]", *sequence));
  EXPECT_FALSE(runParser("[v1.]", *sequence));
  EXPECT_FALSE(runParser("[]", *sequence));

  // Trailing colon.
  EXPECT_FALSE(runParser("[2001:db8::1:]", *sequence));
  EXPECT_FALSE(runParser("[1:2:]", *sequence));
  EXPECT_FALSE(runParser("[:]", *sequence));

  // Missing brackets.
  EXPECT_FALSE(runParser("2001:db8::1", *sequence));
  EXPECT_FALSE(runParser("v1.fe80", *sequence));
}

/**
 * IPvFuture     = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
 */
TEST(UriAbnfTest, IPvFuture)
{
  ft::shared_ptr<SequenceRule> sequence = ipvFutureRule();

  EXPECT_TRUE(runParser("v1.fe80", *sequence));
  EXPECT_TRUE(runParser("v0123456789abcdefABCDEF.fe80", *sequence));
  EXPECT_TRUE(runParser("vFa16F.00", *sequence));
  EXPECT_TRUE(runParser("vF.aBc123-._~!$&'()*+,;=:0XyZ", *sequence));
  EXPECT_TRUE(runParser("v1.:", *sequence));

  // --- Invalid ---
  // Missing part of sequence.
  EXPECT_FALSE(runParser("1.fe80", *sequence));
  EXPECT_FALSE(runParser("v.fe80", *sequence));
  EXPECT_FALSE(runParser("v1-fe80", *sequence));
  EXPECT_FALSE(runParser("v1:", *sequence));

  // Invalid chars.
  EXPECT_FALSE(runParser("vG.fe80", *sequence));
  EXPECT_FALSE(runParser("v1.fe 80", *sequence));
  EXPECT_FALSE(runParser("v1.fe80/", *sequence));
  EXPECT_FALSE(runParser("v1.#:", *sequence));

  ft::shared_ptr<SequenceRule> seqWithEnd = ipvFutureRule();
  seqWithEnd->addRule(ft::make_shared<RangeRule>("\n"));

  // Missing last part of sequence.
  EXPECT_FALSE(runParser("v1.\n", *seqWithEnd));
}

/**
 * IPv6address   =                               6( h16 ":" ) ls32
 *                  /                       "::" 5( h16 ":" ) ls32
 *                  / [               h16 ] "::" 4( h16 ":" ) ls32
 *                  / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
 *                  / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
 *                  / [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
 *                  / [ *4( h16 ":" ) h16 ] "::"              ls32
 *                  / [ *5( h16 ":" ) h16 ] "::"              h16
 *                  / [ *6( h16 ":" ) h16 ] "::"
 */
TEST(UriAbnfTest, IPv6address)
{
  ft::shared_ptr<AlternativeRule> alter = ipv6AddressRule();

  EXPECT_TRUE(runParser("2001:db8::1", *alter));

  EXPECT_TRUE(runParser("2001:0db8:0000:0000:0000:ff00:0042:8329", *alter));
  EXPECT_TRUE(runParser("::0db8:0000:0000:0000:ff00:0042:8329", *alter));
  EXPECT_TRUE(runParser("2001::0000:0000:ff00:0042:8329", *alter));
  EXPECT_TRUE(runParser("2001:0db8::0000:ff00:0042:8329", *alter));
  EXPECT_TRUE(runParser("2001:0db8:85a3::ff00:0042:8329", *alter));
  EXPECT_TRUE(runParser("2001:0db8:85a3:08d3::370:7334", *alter));
  EXPECT_TRUE(runParser("::ffff:192.0.2.128", *alter));
  EXPECT_TRUE(runParser("2001:0db8:85a3:0000:0000:8a2e::7334", *alter));
  EXPECT_TRUE(runParser("::", *alter));

  EXPECT_FALSE(runParser("2001:db8:85a3::8a2e::7334", *alter));
  EXPECT_FALSE(runParser("2001:db8:85a3 ", *alter));
  EXPECT_FALSE(runParser("2001:db8:85a3:0:0:8a2e:370:7334:1234", *alter));
  EXPECT_FALSE(runParser("2001:db8:85a3:z:0:8a2e:370:7334", *alter));
  EXPECT_FALSE(runParser("2001::g", *alter));
  EXPECT_FALSE(runParser("2001:db8:85a3::192.168.1.1.5", *alter));
  EXPECT_FALSE(runParser(":::", *alter));
  EXPECT_FALSE(runParser("2001:db8:::", *alter));
  EXPECT_FALSE(runParser("::ffff:999.999.999.999", *alter));

  // 6( h16 ":" ) ls32
  EXPECT_TRUE(runParser("aaaa:bbbb:cccc:dddd:eeee:ffff:AAAA:BBBB", *alter));
  EXPECT_TRUE(runParser("aaaa:bbbb:cccc:dddd:eeee:ffff:1.2.3.4", *alter));

  // "::" 5( h16 ":" ) ls32
  EXPECT_TRUE(runParser("::aaaa:bbbb:cccc:dddd:eeee:AAAA:BBBB", *alter));
  EXPECT_TRUE(runParser("::aaaa:bbbb:cccc:dddd:eeee:1.2.3.4", *alter));

  // [ h16 ] "::" 4( h16 ":" ) ls32
  EXPECT_TRUE(runParser("1111::aaaa:bbbb:cccc:dddd:AAAA:BBBB", *alter));
  EXPECT_TRUE(runParser("1111::aaaa:bbbb:cccc:dddd:1.2.3.4", *alter));
  EXPECT_TRUE(runParser("::aaaa:bbbb:cccc:dddd:AAAA:BBBB", *alter));

  // [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
  EXPECT_TRUE(runParser("1111:2222::aaaa:bbbb:cccc:AAAA:BBBB", *alter));
  EXPECT_TRUE(runParser("1111:2222::aaaa:bbbb:cccc:1.2.3.4", *alter));
  EXPECT_TRUE(runParser("1111::aaaa:bbbb:cccc:AAAA:BBBB", *alter));
  EXPECT_TRUE(runParser("::aaaa:bbbb:cccc:AAAA:BBBB", *alter));

  // [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
  EXPECT_TRUE(runParser("1111:2222:3333::aaaa:bbbb:AAAA:BBBB", *alter));
  EXPECT_TRUE(runParser("1111:2222:3333::aaaa:bbbb:1.2.3.4", *alter));
  EXPECT_TRUE(runParser("1111:2222::aaaa:bbbb:AAAA:BBBB", *alter));
  EXPECT_TRUE(runParser("1111::aaaa:bbbb:AAAA:BBBB", *alter));
  EXPECT_TRUE(runParser("::aaaa:bbbb:AAAA:BBBB", *alter));

  // [ *3( h16 ":" ) h16 ] "::" h16 ":" ls32
  EXPECT_TRUE(runParser("1111:2222:3333:4444::aaaa:AAAA:BBBB", *alter));
  EXPECT_TRUE(runParser("1111:2222:3333:4444::aaaa:1.2.3.4", *alter));
  EXPECT_TRUE(runParser("1111:2222:3333::aaaa:AAAA:BBBB", *alter));
  EXPECT_TRUE(runParser("1111:2222::aaaa:AAAA:BBBB", *alter));
  EXPECT_TRUE(runParser("1111::aaaa:AAAA:BBBB", *alter));
  EXPECT_TRUE(runParser("::aaaa:AAAA:BBBB", *alter));

  // [ *4( h16 ":" ) h16 ] "::" ls32
  EXPECT_TRUE(runParser("1111:2222:3333:4444:5555::AAAA:BBBB", *alter));
  EXPECT_TRUE(runParser("1111:2222:3333:4444:5555::1.2.3.4", *alter));
  EXPECT_TRUE(runParser("1111:2222:3333:4444::AAAA:BBBB", *alter));
  EXPECT_TRUE(runParser("1111:2222:3333::AAAA:BBBB", *alter));
  EXPECT_TRUE(runParser("1111:2222::AAAA:BBBB", *alter));
  EXPECT_TRUE(runParser("1111::AAAA:BBBB", *alter));
  EXPECT_TRUE(runParser("::AAAA:BBBB", *alter));
  EXPECT_TRUE(runParser("::1.2.3.4", *alter));

  // [ *5( h16 ":" ) h16 ] "::" h16
  EXPECT_TRUE(runParser("1111:2222:3333:4444:5555:6666::aaaa", *alter));
  EXPECT_TRUE(runParser("1111:2222:3333:4444:5555::aaaa", *alter));
  EXPECT_TRUE(runParser("1111:2222:3333:4444::aaaa", *alter));
  EXPECT_TRUE(runParser("1111:2222:3333::aaaa", *alter));
  EXPECT_TRUE(runParser("1111:2222::aaaa", *alter));
  EXPECT_TRUE(runParser("1111::aaaa", *alter));
  EXPECT_TRUE(runParser("::aaaa", *alter));

  // [ *6( h16 ":" ) h16 ] "::"
  EXPECT_TRUE(runParser("1111:2222:3333:4444:5555:6666:7777::", *alter));
  EXPECT_TRUE(runParser("1111:2222:3333:4444:5555:6666::", *alter));
  EXPECT_TRUE(runParser("1111:2222:3333:4444:5555::", *alter));
  EXPECT_TRUE(runParser("1111:2222:3333:4444::", *alter));
  EXPECT_TRUE(runParser("1111:2222:3333::", *alter));
  EXPECT_TRUE(runParser("1111:2222::", *alter));
  EXPECT_TRUE(runParser("1111::", *alter));

  // --- Invalid ---
  // More than 8 h16 groups.
  EXPECT_FALSE(runParser("0:1:2:3:4:5:6:7:8", *alter));

  // Multiple "::".
  EXPECT_FALSE(runParser("2001:db8:85a3::8a2e::7334", *alter));
  EXPECT_FALSE(runParser("2001:db8::85a3:8a2e::7334", *alter));

  // Invalid character.
  EXPECT_FALSE(runParser("2001:db8:85a3:z:0:8a2e:370:7334", *alter));

  // Invalid IPv4 part.
  EXPECT_FALSE(runParser("::ffff:999.999.999.999", *alter));
  EXPECT_FALSE(runParser("2001:db8:85a3::192.168.1.1.5", *alter));

  // Invalid separators or endings.
  EXPECT_FALSE(runParser(":::", *alter));
  EXPECT_FALSE(runParser("2001:db8:::", *alter));
  EXPECT_FALSE(runParser("2001:db8:85a3 ", *alter));
  EXPECT_FALSE(runParser("2001::1: ", *alter));
  EXPECT_FALSE(runParser("1:2: ", *alter));
  EXPECT_FALSE(runParser("0:1:2:3:4:5:6:7:", *alter));
  EXPECT_FALSE(runParser(":2001::1", *alter));
  EXPECT_FALSE(runParser(":2001:1", *alter));
  EXPECT_FALSE(runParser(":0", *alter));

  // Too many h16 groups with "::".
  EXPECT_FALSE(runParser("1:2:3:4:5:6:7:8::", *alter));
  EXPECT_FALSE(runParser("1:2:3:4:5:6:7::8", *alter));
  EXPECT_FALSE(runParser("1:2:3:4:5:6::7:8", *alter));
  EXPECT_FALSE(runParser("1:2:3:4:5::6:7:8", *alter));
  EXPECT_FALSE(runParser("1:2:3:4::5:6:7:8", *alter));
  EXPECT_FALSE(runParser("1:2:3::4:5:6:7:8", *alter));
  EXPECT_FALSE(runParser("1:2::3:4:5:6:7:8", *alter));
  EXPECT_FALSE(runParser("1::2:3:4:5:6:7:8", *alter));
  EXPECT_FALSE(runParser("::1:2:3:4:5:6:7:8", *alter));

  // h16 group too long.
  EXPECT_FALSE(runParser("01234::", *alter));
}

/**
 * h16           = 1*4HEXDIG
 */
TEST(UriAbnfTest, h16)
{
  ft::shared_ptr<RepetitionRule> rep = h16Rule();
  EXPECT_TRUE(runParser("0", *rep));
  EXPECT_TRUE(runParser("A", *rep));
  EXPECT_TRUE(runParser("f", *rep));
  EXPECT_TRUE(runParser("09", *rep));
  EXPECT_TRUE(runParser("1aF", *rep));
  EXPECT_TRUE(runParser("abcd", *rep));
  EXPECT_TRUE(runParser("ABCD", *rep));
  EXPECT_TRUE(runParser("0001", *rep));
  EXPECT_TRUE(runParser("FFFF", *rep));
  EXPECT_TRUE(runParser("9AfE", *rep));

  // Invalid
  EXPECT_FALSE(runParser("12345", *rep));
  EXPECT_FALSE(runParser("G", *rep));
  EXPECT_FALSE(runParser("0x1", *rep));
  EXPECT_FALSE(runParser("1.2", *rep));
  EXPECT_FALSE(runParser("1g", *rep));
}

/**
 * ls32          = ( h16 ":" h16 ) / IPv4address
 */
TEST(UriAbnfTest, ls32)
{
  ft::shared_ptr<AlternativeRule> alter = ls32Rule();
  EXPECT_TRUE(runParser("abcd:1234", *alter));
  EXPECT_TRUE(runParser("0:0", *alter));
  EXPECT_TRUE(runParser("FFFF:FFFF", *alter));
  EXPECT_TRUE(runParser("0001:0002", *alter));
  EXPECT_TRUE(runParser("A:B", *alter));
  EXPECT_TRUE(runParser("9a9:FfF", *alter));

  EXPECT_TRUE(runParser("0.0.0.0", *alter));
  EXPECT_TRUE(runParser("127.0.0.1", *alter));
  EXPECT_TRUE(runParser("255.255.255.255", *alter));
  EXPECT_TRUE(runParser("192.168.1.42", *alter));
  EXPECT_TRUE(runParser("10.20.30.40", *alter));

  // Invalid
  EXPECT_FALSE(runParser("abcd1234", *alter));
  EXPECT_FALSE(runParser("abcd:12345", *alter));
  EXPECT_FALSE(runParser("abcd::1234", *alter));
  EXPECT_FALSE(runParser("abcd:g123", *alter));
  EXPECT_FALSE(runParser("256.256.256.256", *alter));
  EXPECT_FALSE(runParser("192.168.1 ", *alter));
  EXPECT_FALSE(runParser("abcd:192.168.1.1", *alter));
}

/**
 * IPv4address   = dec-octet "." dec-octet "." dec-octet "." dec-octet
 */
TEST(UriAbnfTest, IPv4address_Valid)
{
  ft::shared_ptr<SequenceRule> sequence = ipv4AddressRule();

  EXPECT_TRUE(runParser("0.0.0.0", *sequence));
  EXPECT_TRUE(runParser("9.9.9.9", *sequence));
  EXPECT_TRUE(runParser("10.0.0.1", *sequence));
  EXPECT_TRUE(runParser("127.0.0.1", *sequence));
  EXPECT_TRUE(runParser("192.168.1.1", *sequence));
  EXPECT_TRUE(runParser("255.255.255.255", *sequence));

  // Invalid
  EXPECT_FALSE(runParser("256.0.0.1", *sequence));
  EXPECT_FALSE(runParser("1.2.3 ", *sequence));
  EXPECT_FALSE(runParser("1.2.3.4.5", *sequence));
  EXPECT_FALSE(runParser("01.2.3.4", *sequence));
  EXPECT_FALSE(runParser("192.168.1. ", *sequence));
  EXPECT_FALSE(runParser("abc.def.ghi.jkl", *sequence));
}

/**
 * dec-octet     = DIGIT                 ; 0-9
 *               / %x31-39 DIGIT         ; 10-99
 *               / "1" 2DIGIT            ; 100-199
 *               / "2" %x30-34 DIGIT     ; 200-249
 *               / "25" %x30-35          ; 250-255
 */
TEST(UriAbnfTest, DecOctet)
{
  SequenceRule sequence;
  sequence.addRule(decOctetRule());

  // 0-9
  EXPECT_TRUE(runParser("1", sequence));
  EXPECT_TRUE(runParser("9", sequence));

  // 10-99
  EXPECT_TRUE(runParser("10", sequence));
  EXPECT_TRUE(runParser("99", sequence));

  // 100-199
  EXPECT_TRUE(runParser("100", sequence));
  EXPECT_TRUE(runParser("199", sequence));

  // 200-249
  EXPECT_TRUE(runParser("200", sequence));
  EXPECT_TRUE(runParser("249", sequence));

  // 250-255
  EXPECT_TRUE(runParser("250", sequence));
  EXPECT_TRUE(runParser("255", sequence));
}

/**
 * reg-name      = *( unreserved / pct-encoded / sub-delims )
 */
TEST(UriAbnfTest, RegName)
{
  SequenceRule sequence;
  sequence.addRule(regNameRule());
  EXPECT_TRUE(runParser("a", sequence));
  EXPECT_TRUE(runParser("a%20", sequence));
  EXPECT_TRUE(runParser("a%20b", sequence));
  EXPECT_TRUE(runParser("a%20b!!!=", sequence));
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
TEST(UriAbnfTest, Path)
{
  ft::shared_ptr<AlternativeRule> alter = pathRule();
  // path-abempty    ; begins with "/" or is empty
  EXPECT_TRUE(runParser("", *alter));
  EXPECT_TRUE(runParser("/", *alter));
  EXPECT_TRUE(runParser("//", *alter));
  EXPECT_TRUE(runParser("/abc/def", *alter));
  EXPECT_TRUE(runParser("/abc/", *alter));

  // path-absolute   ; begins with "/" but not "//"
  EXPECT_TRUE(runParser("/abc", *alter));
  EXPECT_TRUE(runParser("/abc/def", *alter));
  EXPECT_TRUE(runParser("/abc/", *alter));
  EXPECT_TRUE(runParser("/", *alter));

  // path-noscheme   ; begins with a non-colon segment
  EXPECT_TRUE(runParser("abc", *alter));
  EXPECT_TRUE(runParser("abc/def", *alter));
  EXPECT_TRUE(runParser("abc/", *alter));
  EXPECT_TRUE(runParser("@user/data", *alter));
  EXPECT_TRUE(runParser("file/name", *alter));

  // path-rootless   ; begins with a segment
  EXPECT_TRUE(runParser("foo:bar", *alter));
  EXPECT_TRUE(runParser(":abc", *alter));
  EXPECT_TRUE(runParser("a:b/c", *alter));
  EXPECT_TRUE(runParser("@user", *alter));

  // Invalid
  EXPECT_FALSE(runParser("?pub", *alter));
}

/**
 * path-abempty  = *( "/" segment )
 */
TEST(UriAbnfTest, PathAbEmpty)
{
  SequenceRule sequence;
  ft::shared_ptr<RepetitionRule> rep = pathAbEmptyRule(Undefined);
  sequence.addRule(rep);
  EXPECT_TRUE(runParser("/abc/def", sequence));
  EXPECT_TRUE(runParser("/abc/def//", sequence));
  EXPECT_TRUE(runParser("///", sequence));
}

/**
 * path-absolute = "/" [ segment-nz *( "/" segment ) ]
 */
TEST(UriAbnfTest, PathAbsolute)
{
  ft::shared_ptr<SequenceRule> sequence = pathAbsoluteRule(Undefined);
  EXPECT_TRUE(runParser("/abc/def", *sequence));
  EXPECT_TRUE(runParser("/abc/def///a", *sequence));
  EXPECT_TRUE(runParser("/", *sequence));

  // Invalid
  EXPECT_FALSE(runParser("///", *sequence));
  EXPECT_FALSE(runParser("//abc/", *sequence));
}

/**
 * path-noscheme = segment-nz-nc *( "/" segment )
 */
TEST(UriAbnfTest, PathNoScheme)
{
  ft::shared_ptr<SequenceRule> sequence = pathNoSchemeRule(Undefined);
  EXPECT_TRUE(runParser("abc", *sequence));
  EXPECT_TRUE(runParser("abc/def", *sequence));
  EXPECT_TRUE(runParser("abc/def/ghi", *sequence));
  EXPECT_TRUE(runParser("@user/data", *sequence));
  EXPECT_TRUE(runParser("%20info/more", *sequence));
  EXPECT_TRUE(runParser("!$&'()*+,;=@/file", *sequence));
  EXPECT_TRUE(runParser("abc/", *sequence));

  // Invalid
  // must not start with '/'
  EXPECT_FALSE(runParser("/abc", *sequence));
  // segment-nz-nc can’t contain ':'
  EXPECT_FALSE(runParser(":abc", *sequence));
  // ':' not allowed in first segment
  EXPECT_FALSE(runParser("a:b", *sequence));
  // same reason: first char must not be '/'
  EXPECT_FALSE(runParser("//abc", *sequence));
  EXPECT_FALSE(runParser("///", *sequence));
  EXPECT_FALSE(runParser("//abc/", *sequence));
  EXPECT_FALSE(runParser("?pub", *sequence));
  EXPECT_FALSE(runParser("/a/?pub", *sequence));
  EXPECT_FALSE(runParser("/a/pub?", *sequence));
}

/**
 * path-rootless = segment-nz *( "/" segment )
 */
TEST(UriAbnfTest, PathRootless)
{
  ft::shared_ptr<SequenceRule> sequence = pathRootlessRule(Undefined);

  EXPECT_TRUE(runParser("abc", *sequence));
  EXPECT_TRUE(runParser("abc/def", *sequence));
  EXPECT_TRUE(runParser("abc/def/ghi", *sequence));
  EXPECT_TRUE(runParser("foo:bar", *sequence));
  EXPECT_TRUE(runParser("a:b/c", *sequence));
  EXPECT_TRUE(runParser("@user/data", *sequence));
  EXPECT_TRUE(runParser("%20info/more", *sequence));
  EXPECT_TRUE(runParser(":abc", *sequence));
  EXPECT_TRUE(runParser("abc/", *sequence));

  // Invalid
  EXPECT_FALSE(runParser("/abc", *sequence));
  EXPECT_FALSE(runParser("//abc", *sequence));
}

/**
 * segment      = *pchar
 */
TEST(UriAbnfTest, Segment)
{
  SequenceRule sequence;
  ft::shared_ptr<RepetitionRule> rep = segmentRule();
  sequence.addRule(rep);
  EXPECT_TRUE(runParser("", sequence));
  EXPECT_EQ(rep->getReps(), 0);
  EXPECT_TRUE(runParser("a", sequence));
  EXPECT_EQ(rep->getReps(), 1);
  EXPECT_TRUE(runParser("1", sequence));
  EXPECT_EQ(rep->getReps(), 1);
  EXPECT_TRUE(runParser("abc%20", sequence));
  EXPECT_EQ(rep->getReps(), 4);
  EXPECT_TRUE(runParser("foo:bar", sequence));
  EXPECT_EQ(rep->getReps(), 7);
  EXPECT_TRUE(runParser("test@site", sequence));
  EXPECT_EQ(rep->getReps(), 9);
  EXPECT_TRUE(runParser("-._~", sequence));
  EXPECT_EQ(rep->getReps(), 4);

  // Invalid tests make more sense in a sequence combination
  // this will be done with the path tests
}

/**
 * segment-nz    = 1*pchar
 */
TEST(UriAbnfTest, SegmentNz)
{
  SequenceRule sequence;
  ft::shared_ptr<RepetitionRule> rep = segmentNzRule();
  sequence.addRule(rep);
  EXPECT_TRUE(runParser("a", sequence));
  EXPECT_EQ(rep->getReps(), 1);
  EXPECT_TRUE(runParser("1", sequence));
  EXPECT_EQ(rep->getReps(), 1);
  EXPECT_TRUE(runParser("abc%20", sequence));
  EXPECT_EQ(rep->getReps(), 4);
  EXPECT_TRUE(runParser("foo:bar", sequence));
  EXPECT_EQ(rep->getReps(), 7);
  EXPECT_TRUE(runParser("test@site", sequence));
  EXPECT_EQ(rep->getReps(), 9);
  EXPECT_TRUE(runParser("-._~", sequence));
  EXPECT_EQ(rep->getReps(), 4);

  // Invalid tests make more sense in a sequence combination
  // this will be done with the path tests
}

/**
   segment-nz-nc = 1*( unreserved / pct-encoded / sub-delims / "@" )
                   ; non-zero-length segment without any colon ":"
 */
TEST(UriAbnfTest, SegmentNzNc)
{
  SequenceRule sequence;
  ft::shared_ptr<RepetitionRule> rep = segmentNzNcRule();
  sequence.addRule(rep);
  EXPECT_TRUE(runParser("a", sequence));
  EXPECT_EQ(rep->getReps(), 1);
  EXPECT_TRUE(runParser("abc", sequence));
  EXPECT_EQ(rep->getReps(), 3);
  EXPECT_TRUE(runParser("@user", sequence));
  EXPECT_EQ(rep->getReps(), 5);
  EXPECT_TRUE(runParser("%20", sequence));
  EXPECT_EQ(rep->getReps(), 1);
  EXPECT_TRUE(runParser("abc%7E", sequence));
  EXPECT_EQ(rep->getReps(), 4);
  EXPECT_TRUE(runParser("!$&'()*+,;=@", sequence));
  EXPECT_EQ(rep->getReps(), 12);
  EXPECT_TRUE(runParser("abc%20@id", sequence));
  EXPECT_EQ(rep->getReps(), 7);

  // Invalid
  // valid until : so cout is 3
  EXPECT_FALSE(runParser("foo:bar", sequence));
  EXPECT_EQ(rep->getReps(), 3);
  EXPECT_FALSE(runParser("a:b", sequence));
  EXPECT_EQ(rep->getReps(), 1);
  EXPECT_FALSE(runParser(":", sequence));
  EXPECT_EQ(rep->getReps(), 0);
  EXPECT_FALSE(runParser("?", sequence));
  EXPECT_EQ(rep->getReps(), 0);
}

/**
 * pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
 */
TEST(UriAbnfTest, Pchar)
{
  {
    SequenceRule sequence;
    ft::shared_ptr<RepetitionRule> rep =
      ft::make_shared<RepetitionRule>(pcharRule());
    rep->setMin(3);
    sequence.addRule(rep);
    EXPECT_TRUE(runParser("user@test", sequence));
    EXPECT_TRUE(runParser("%0F%0F%0F", sequence));
    EXPECT_TRUE(runParser("abc%0F!ac", sequence));
  }

  {
    // Invalid
    SequenceRule sequence;
    ft::shared_ptr<RepetitionRule> rep =
      ft::make_shared<RepetitionRule>(pcharRule());
    rep->setMin(4);
    sequence.addRule(rep);
    EXPECT_FALSE(runParser("abc%1Z!ac", sequence));
  }
}

/**
 * query = *( pchar / "/" / "?" )
 */
TEST(UriAbnfTest, Query)
{
  SequenceRule sequence;
  sequence.addRule(queryRule());
  EXPECT_TRUE(runParser("abc%0F!ac/abc/?", sequence));
  EXPECT_TRUE(runParser("?abc%0F!ac/abc/?", sequence));
  EXPECT_TRUE(runParser("???", sequence));
}

/**
 * fragment = *( pchar / "/" / "?" )
 */
TEST(UriAbnfTest, Fragment)
{
  SequenceRule sequence;
  sequence.addRule(fragmentRule());
  EXPECT_TRUE(runParser("abc%0F!ac/abc/?", sequence));
}

/**
 * pct-encoded   = "%" HEXDIG HEXDIG
 */
TEST(UriAbnfTest, PercentEncoded)
{
  SequenceRule sequence;
  sequence.addRule(ft::make_shared<LiteralRule>("%"));
  sequence.addRule(ft::make_shared<RangeRule>(http::isHexDigit));
  sequence.addRule(ft::make_shared<RangeRule>(http::isHexDigit));

  // Valid
  EXPECT_TRUE(runParser("%aB", sequence));
  EXPECT_TRUE(runParser("%0F", sequence));

  // Invalid
  EXPECT_FALSE(runParser("%A ", sequence));
  EXPECT_FALSE(runParser("%G1", sequence));
  EXPECT_FALSE(runParser("%1Z", sequence));
}

/**
 * unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
 */
TEST(UriAbnfTest, ValidUnreserved)
{
  EXPECT_TRUE(http::isUnreserved('A'));
  EXPECT_TRUE(http::isUnreserved('1'));
  EXPECT_TRUE(http::isUnreserved('-'));
  EXPECT_TRUE(http::isUnreserved('.'));
  EXPECT_TRUE(http::isUnreserved('_'));
  EXPECT_TRUE(http::isUnreserved('~'));
}

/**
 * reserved = gen-delims / sub-delims
 */
TEST(UriAbnfTest, ValidReserved)
{
  EXPECT_TRUE(http::isReserved(':'));
  EXPECT_TRUE(http::isReserved('/'));
  EXPECT_TRUE(http::isReserved('?'));
  EXPECT_TRUE(http::isReserved('#'));
  EXPECT_TRUE(http::isReserved('['));
  EXPECT_TRUE(http::isReserved(']'));
  EXPECT_TRUE(http::isReserved('@'));
  EXPECT_TRUE(http::isReserved('!'));
  EXPECT_TRUE(http::isReserved('$'));
  EXPECT_TRUE(http::isReserved('&'));
  EXPECT_TRUE(http::isReserved('\''));
  EXPECT_TRUE(http::isReserved('('));
  EXPECT_TRUE(http::isReserved(')'));
  EXPECT_TRUE(http::isReserved('*'));
  EXPECT_TRUE(http::isReserved('+'));
  EXPECT_TRUE(http::isReserved(','));
  EXPECT_TRUE(http::isReserved(';'));
  EXPECT_TRUE(http::isReserved('='));
}

/**
 * gen-delims = ":" / "/" / "?" / "#" / "[" / "]" / "@"
 */
TEST(UriAbnfTest, ValidGenDelims)
{
  EXPECT_TRUE(http::isGenDelim(':'));
  EXPECT_TRUE(http::isGenDelim('/'));
  EXPECT_TRUE(http::isGenDelim('?'));
  EXPECT_TRUE(http::isGenDelim('#'));
  EXPECT_TRUE(http::isGenDelim('['));
  EXPECT_TRUE(http::isGenDelim(']'));
  EXPECT_TRUE(http::isGenDelim('@'));
}

/**
 * sub-delims = "!" / "$" / "&" / "'" / "(" / ")" /
 *              "*" / "+" / "," / ";" / "="
 */
TEST(UriAbnfTest, ValidSubDelims)
{
  EXPECT_TRUE(http::isSubDelim('!'));
  EXPECT_TRUE(http::isSubDelim('$'));
  EXPECT_TRUE(http::isSubDelim('&'));
  EXPECT_TRUE(http::isSubDelim('\''));
  EXPECT_TRUE(http::isSubDelim('('));
  EXPECT_TRUE(http::isSubDelim(')'));
  EXPECT_TRUE(http::isSubDelim('*'));
  EXPECT_TRUE(http::isSubDelim('+'));
  EXPECT_TRUE(http::isSubDelim(','));
  EXPECT_TRUE(http::isSubDelim(';'));
  EXPECT_TRUE(http::isSubDelim('='));
}

// NOLINTEND

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
