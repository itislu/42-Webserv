#include "libftpp/memory.hpp"
#include "utils/abnfRules/SequenceRule.hpp"
#include <http/abnfRules/headerLineRules.hpp>
#include <utils/Buffer.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/RepetitionRule.hpp>
#include <utils/abnfRules/Rule.hpp>

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
  Buffer buffer;
  buffer.add(str);
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

TEST(HeaderLinesTester, FieldValueRule)
{
  const ft::shared_ptr<RepetitionRule> rule = fieldValueRule();

  EXPECT_TRUE(runParser("test, hallo", *rule));
}

TEST(HeaderLinesTester, FieldLineRule)
{
  const ft::shared_ptr<SequenceRule> rule = fieldLineRule();

  Rule::debugPrint = true;
  EXPECT_TRUE(runParser("host: test, hallo", *rule));
  Rule::debugPrint = false;
}

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
