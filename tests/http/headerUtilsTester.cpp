#include <http/headerUtils.hpp>

#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(HeaderUtilsTester, ConvertHeaderList)
{
  // 2 elements
  std::vector<std::string> output = { "e1", "e2" };
  EXPECT_EQ(convertHeaderList("e1, e2"), output);
  EXPECT_EQ(convertHeaderList("e1, e2 , , , "), output);
  EXPECT_EQ(convertHeaderList(", , , e1, e2 , , , "), output);
  EXPECT_EQ(convertHeaderList(",e1,e2,"), output);
  EXPECT_EQ(convertHeaderList("e1,e2,"), output);

  // 3 elements
  output = { "e1", "e2", "e  3" };
  EXPECT_EQ(convertHeaderList("e1,e2,e  3, ,"), output);

  // empty
  EXPECT_EQ(convertHeaderList(",,,,"), std::vector<std::string>());
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
