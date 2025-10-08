#include <http/TokenFinder.hpp>
#include <http/http.hpp>
#include <utils/Buffer.hpp>
#include <string>

#include <gtest/gtest.h>

TEST(TokenFinderTester, BasicTest)
{
  TokenFinder finder;
  Buffer buffer;
  buffer.add("GET TEST\r\n");


  finder.initTokenFinder(&buffer, http::SP);
  EXPECT_EQ(finder.buffContainsToken(), true);
  EXPECT_EQ(finder.getStrUntilToken(), "GET");

  finder.initTokenFinder(&buffer, http::CRLF);
  EXPECT_EQ(finder.buffContainsToken(), true);
  EXPECT_EQ(finder.getStrUntilToken(), "GET TEST");
}

TEST(TokenFinderTester, GrowingBufferTest)
{
  TokenFinder finder;
  Buffer buffer;
  buffer.add("GET");


  finder.initTokenFinder(&buffer, http::SP);
  EXPECT_EQ(finder.buffContainsToken(), false);
  buffer.add(" ");
  EXPECT_EQ(finder.buffContainsToken(), true);
  EXPECT_EQ(finder.getStrUntilToken(), "GET");

  finder.initTokenFinder(&buffer, http::CRLF);
  buffer.add("TES");
  EXPECT_EQ(finder.buffContainsToken(), false);
  buffer.add("T\r\n");
  EXPECT_EQ(finder.buffContainsToken(), true);
  EXPECT_EQ(finder.getStrUntilToken(), "GET TEST");
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
