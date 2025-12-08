#include "utils/buffer/SmartBuffer.hpp"
#include <http/abnfRules/requestLineRules.hpp>
#include <libftpp/memory.hpp>
#include <utils/BufferReader.hpp>
#include <utils/abnfRules/Rule.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

#include <gtest/gtest.h>
#include <string>

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

} // namespace

/**
 * The path is terminated
 * by the first question mark ("?") or number sign ("#") character, or
 * by the end of the URI.
 */
TEST(RequestLineRulesTester, PathPartRule)
{
  const ft::unique_ptr<SequenceRule> rule = pathPartRule();

  EXPECT_TRUE(runParser("/test/ ", *rule));
  EXPECT_TRUE(runParser("/t/", *rule));
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
