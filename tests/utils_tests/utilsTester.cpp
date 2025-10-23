#include <gtest/gtest.h>

TEST(TESTER_NAME, arraySize2)
{
  // EXPECT_EQ(parseArgs(), 1);
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
