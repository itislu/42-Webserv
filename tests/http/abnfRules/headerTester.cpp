#include <http/abnfRules/headerRules.hpp>
#include <http/abnfRules/ruleUtils.hpp>
#include <libftpp/memory.hpp>
#include <utils/abnfRules/SequenceRule.hpp>

#include <gtest/gtest.h>

/**
 * Host = uri-host [ ":" port ]
 * host = IP-literal / IPv4address / reg-name
 * port = *DIGIT
 */
TEST(HeaderTester, HostHeader)
{
  const ft::shared_ptr<SequenceRule> rule = fieldLineWrapper(hostHeaderRule());

  EXPECT_TRUE(isValidString(*rule, "webserv:8080\r\n"));
  EXPECT_TRUE(isValidString(*rule, "webserv:8080 \r\n"));
  EXPECT_TRUE(isValidString(*rule, "webserv\r\n"));
  EXPECT_TRUE(isValidString(*rule, "webserv:\r\n"));

  EXPECT_FALSE(isValidString(*rule, "webserv:a\r\n"));
}

/**
 * Content-Length = 1*DIGIT
 */
TEST(HeaderTester, ContentLength)
{
  const ft::shared_ptr<SequenceRule> rule =
    fieldLineWrapper(contentLengthRule());

  EXPECT_TRUE(isValidString(*rule, "123\r\n"));
  EXPECT_TRUE(isValidString(*rule, "  123  \r\n"));
  EXPECT_TRUE(isValidString(*rule, "0\r\n"));
  EXPECT_TRUE(isValidString(*rule, "2147483647\r\n"));

  EXPECT_FALSE(isValidString(*rule, " a \r\n"));
  EXPECT_FALSE(isValidString(*rule, " 123 123 \r\n"));
  EXPECT_FALSE(isValidString(*rule, " 123 a \r\n"));
  EXPECT_FALSE(isValidString(*rule, " 123a \r\n"));
}

/**
 * Transfer-Encoding = [ transfer-coding *( OWS "," OWS transfer-coding ) ]
 * transfer-coding = token *( OWS ";" OWS transfer-parameter )
 * transfer-parameter = token BWS "=" BWS ( token / quoted-string )
 */
TEST(HeaderTester, TransferEncoding)
{
  const ft::shared_ptr<SequenceRule> rule =
    fieldLineWrapper(transferEncodingRule());
  // Basic tokens
  EXPECT_TRUE(isValidString(*rule, "chunked\r\n"));
  EXPECT_TRUE(isValidString(*rule, "gzip\r\n"));
  EXPECT_TRUE(isValidString(*rule, "compress\r\n"));
  EXPECT_TRUE(isValidString(*rule, "br\r\n"));

  // Multiple codings
  EXPECT_TRUE(isValidString(*rule, "gzip, chunked\r\n"));
  EXPECT_TRUE(isValidString(*rule, "gzip , chunked\r\n"));
  EXPECT_TRUE(isValidString(*rule, "gzip,chunked,compress\r\n"));

  // Single parameter
  EXPECT_TRUE(isValidString(*rule, "gzip;q=1\r\n"));
  EXPECT_TRUE(isValidString(*rule, "gzip ; q = 0\r\n"));
  EXPECT_TRUE(isValidString(*rule, "gzip; level=9\r\n"));

  // Multiple parameters
  EXPECT_TRUE(isValidString(*rule, "gzip; q=1 ; level=3\r\n"));
  EXPECT_TRUE(isValidString(*rule, "br; a=b; c=d\r\n"));
  EXPECT_TRUE(
    isValidString(*rule, "chunked;foo=bar;test=\"quoted value\"\r\n"));

  // Mixed codings with parameters
  EXPECT_TRUE(isValidString(*rule, "gzip;q=1, chunked\r\n"));
  EXPECT_TRUE(isValidString(*rule, "gzip;q=1 , chunked ;test=1\r\n"));
  EXPECT_TRUE(
    isValidString(*rule, "gzip ; q = 0.5 , br ; x = y ; z = \"ok\"\r\n"));

  // Quoted-string parameter
  EXPECT_TRUE(isValidString(*rule, "gzip; desc=\"hello world\"\r\n"));
  EXPECT_TRUE(isValidString(*rule, "compress; note=\"A-B-C\"\r\n"));

  // Optional empty (allowed by [ … ])
  EXPECT_TRUE(isValidString(*rule, "\r\n"));
  EXPECT_TRUE(isValidString(*rule, " \r\n"));

  // Empty list
  EXPECT_TRUE(isValidString(*rule, ",,,,\r\n"));
  EXPECT_TRUE(isValidString(*rule, ",\r\n"));
  EXPECT_TRUE(isValidString(*rule, " , \r\n"));
  EXPECT_TRUE(isValidString(*rule, " , , , ,  \r\n"));
  EXPECT_TRUE(isValidString(*rule, " , chunked \r\n"));
  EXPECT_TRUE(isValidString(*rule, " , chunked , \r\n"));
  EXPECT_TRUE(isValidString(*rule, " , chunked,test \r\n"));
}

/**
 * Transfer-Encoding = [ transfer-coding *( OWS "," OWS transfer-coding ) ]
 * transfer-coding = token *( OWS ";" OWS transfer-parameter )
 * transfer-parameter = token BWS "=" BWS ( token / quoted-string )
 */
TEST(HeaderTester, TransferEncodingInvalid)
{
  const ft::shared_ptr<SequenceRule> rule =
    fieldLineWrapper(transferEncodingRule());

  EXPECT_FALSE(isValidString(*rule, "gzip=\r\n"));
  EXPECT_FALSE(isValidString(*rule, "gzip;\r\n"));
  EXPECT_FALSE(isValidString(*rule, "gzip;=\r\n"));
  EXPECT_FALSE(isValidString(*rule, "gzip;foo=\r\n"));
  EXPECT_FALSE(isValidString(*rule, "gzip;foo= \r\n"));
  EXPECT_FALSE(isValidString(*rule, "g zip\r\n"));
  EXPECT_FALSE(isValidString(*rule, "chunked test=1\r\n"));
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
