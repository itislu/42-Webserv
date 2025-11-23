#include <utils/SmartBuffer.hpp>

#include <gtest/gtest.h>
#include <string>

TEST(SmartBufferTester, AppendString)
{
  const std::string input = "0123456789Alaaaaaaaaaaaaaaaaaarm";

  SmartBuffer filebuffer;
  filebuffer.append(input);

  std::string result;
  while (filebuffer.size() > 0) {
    const std::string front = filebuffer.consumeFront(1);
    result.append(front);
  }
  EXPECT_EQ(result, input);
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
