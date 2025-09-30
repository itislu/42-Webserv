#include "client/Client.hpp"
#include "http/http.hpp"
#include "utils/tryGetStrUntil.hpp"
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

TEST(TESTER_NAME, tryGetStrUntil)
{
  std::string line = "GET TEST\r\n";
  Client::Buffer buffer(line.begin(), line.end());
  Client::Buffer tokens;
  
  tokens = { ' ' };
  EXPECT_EQ(tryGetStrUntil(buffer, 0, tokens), "GET");

  tokens = { http::CR, http::LF };
  EXPECT_EQ(tryGetStrUntil(buffer, 4, tokens), "TEST");

  line = "GET";
  buffer = Client::Buffer(line.begin(), line.end());
  tokens = { ' ' };
  EXPECT_THROW(tryGetStrUntil(buffer, 0, tokens), std::out_of_range);
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
