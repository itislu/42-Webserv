#include "utils/SmartBuffer.hpp"
#include <http/abnfRules/http11Rules.hpp>
#include <libftpp/memory.hpp>
#include <utils/Buffer.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/LiteralRule.hpp>
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
  BufferReader reader = BufferReader();
  reader.init(&buffer);
  rule.setBufferReader(&reader);
  rule.reset();
  bool matches = rule.matches();
  if (!reader.reachedEnd()) {
    matches = false;
  }
  return matches;
}
}

/**
 * origin-form = absolute-path [ "?" query ]
 */
TEST(Http11RulesTester, OriginForm)
{
  const ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();
  seq->addRule(originFormRule());
  seq->addRule(ft::make_shared<LiteralRule>(" "));

  EXPECT_TRUE(runParser("/where?q=now ", *seq));
  EXPECT_TRUE(runParser("/index.html ", *seq));
  EXPECT_TRUE(runParser("/ ", *seq));
  EXPECT_TRUE(runParser("/search?q=openai ", *seq));
  EXPECT_TRUE(runParser("/api/data?user=42&type=json&debug=true ", *seq));
  EXPECT_TRUE(runParser("/v1/users/profile/edit ", *seq));
  EXPECT_TRUE(runParser("/file%20name%20with%20spaces.txt ", *seq));
  EXPECT_TRUE(runParser("/path;param=value?q=1 ", *seq));
  EXPECT_TRUE(runParser("/endpoint? ", *seq));
  EXPECT_TRUE(runParser("/folder/ ", *seq));
  EXPECT_TRUE(
    runParser("/app/v2/items/list?page=3&sort=desc&filter=on ", *seq));
}

/*
 * origin-form = absolute-path [ "?" query ]
 */
TEST(Http11RulesTester, OriginFormInvalidCases)
{
  const ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();
  seq->addRule(originFormRule());
  seq->addRule(ft::make_shared<LiteralRule>(" "));

  EXPECT_FALSE(runParser("where?q=now ", *seq));
  EXPECT_FALSE(runParser(":/bad/path ", *seq));
  EXPECT_FALSE(runParser("/path with space ", *seq));
  EXPECT_FALSE(runParser("/path?invalid|char ", *seq));
  EXPECT_FALSE(runParser("/%ZZinvalidpct ", *seq));
  EXPECT_FALSE(runParser("/[illegal]chars ", *seq));
  EXPECT_FALSE(runParser("/path#fragment ", *seq));
  EXPECT_FALSE(runParser("/?q=white space ", *seq));
  EXPECT_FALSE(runParser("?onlyquery ", *seq));
  EXPECT_FALSE(runParser("/% ", *seq));
}

/*
 * absolute-form = absolute-URI
 * absolute-URI  = scheme ":" hier-part [ "?" query ]
 */
TEST(Http11RulesTester, AbsoluteForm)
{
  const ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();
  seq->addRule(absoluteFormRule());
  seq->addRule(ft::make_shared<LiteralRule>(" "));

  EXPECT_TRUE(
    runParser("http://www.example.org/pub/WWW/TheProject.html ", *seq));
  EXPECT_TRUE(runParser("http://example.com/index.html ", *seq));
  EXPECT_TRUE(runParser("https://example.org/path/to/page ", *seq));
  EXPECT_TRUE(runParser("http://example.com:8080/ ", *seq));
  EXPECT_TRUE(runParser("https://example.com/api/v1/resource?id=42 ", *seq));
  EXPECT_TRUE(runParser("http://localhost/test?x=1&y=2 ", *seq));
  EXPECT_TRUE(runParser("ftp://ftp.example.net/files/data.zip ", *seq));
  EXPECT_TRUE(
    runParser("https://user:pass@example.com:8443/secure/area ", *seq));
  EXPECT_TRUE(runParser("http://example.com/path%20with%20spaces?q=1 ", *seq));
  EXPECT_TRUE(runParser("https://example.com/a/b/c?param=value&flag ", *seq));
  EXPECT_TRUE(runParser("http://127.0.0.1:8000/status?verbose=true ", *seq));
  EXPECT_TRUE(runParser(
    "https://example.org/long/path/to/resource?one=1&two=2&three=3 ", *seq));
}

/*
 * absolute-form = absolute-URI
 * absolute-URI  = scheme ":" hier-part [ "?" query ]
 */
TEST(Http11RulesTester, AbsoluteFormInvalid)
{
  const ft::shared_ptr<SequenceRule> seq = ft::make_shared<SequenceRule>();
  seq->addRule(absoluteFormRule());
  seq->addRule(ft::make_shared<LiteralRule>(" "));

  EXPECT_FALSE(runParser("http//example.com/index.html ", *seq));
  EXPECT_FALSE(runParser("://example.com/ ", *seq));
  EXPECT_FALSE(runParser("https//:example.com ", *seq));
  EXPECT_FALSE(runParser("example.com/index.html ", *seq));
  EXPECT_FALSE(runParser("http://exa mple.com/path ", *seq));
  EXPECT_FALSE(runParser("http://example.com/space in path ", *seq));
  EXPECT_FALSE(runParser("https://example.com:port/path ", *seq));
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
