#include "utils/SmartBuffer.hpp"
#include <http/abnfRules/headerLineRules.hpp>
#include <libftpp/memory.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/RangeRule.hpp>
#include <utils/abnfRules/Rule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

#include <cctype>
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

/**
 * field-content = field-vchar
 *                 [ 1*( SP / HTAB / field-vchar ) ]
 */
TEST(HeaderLinesTester, FieldContentRule)
{
  const ft::shared_ptr<SequenceRule> rule = fieldContentRule();
  rule->addRule(ft::make_shared<RangeRule>("\n"));

  EXPECT_TRUE(runParser("tes t\n", *rule));
  EXPECT_TRUE(runParser("test  \n", *rule));
}

/**
 * field-value = *field-content
 */
TEST(HeaderLinesTester, FieldValueRule)
{
  const ft::shared_ptr<SequenceRule> seqRule = ft::make_shared<SequenceRule>();
  seqRule->addRule(fieldValueRule());
  seqRule->addRule(ft::make_shared<RangeRule>("\n"));

  EXPECT_TRUE(runParser("test, hallo\n", *seqRule));
}

/**
 * field-line   = field-name ":" OWS field-value OWS
 */
TEST(HeaderLinesTester, FieldLineRule)
{
  const ft::shared_ptr<SequenceRule> seqRule = ft::make_shared<SequenceRule>();
  seqRule->addRule(fieldLineRule());
  seqRule->addRule(ft::make_shared<RangeRule>("\n"));

  EXPECT_TRUE(runParser("host: test, hallo\n", *seqRule));
  EXPECT_TRUE(runParser("host: test, hallo \n", *seqRule));
  EXPECT_TRUE(runParser("host:test, hallo \n", *seqRule));

  // Invalid
  EXPECT_FALSE(runParser("host :test, hallo \n", *seqRule));
}

/**
 * field-line CRLF
 */
TEST(HeaderLinesTester, FieldLinePartRule)
{
  const ft::unique_ptr<SequenceRule> rule = fieldLinePartRule();

  EXPECT_TRUE(runParser("host: test, hallo\r\n", *rule));
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
