#include <libftpp/expected.hpp>
#include <utils/FileBuffer.hpp>
#include <utils/SmartBuffer.hpp>

#include <gtest/gtest.h>
#include <iostream>
#include <string>

TEST(SmartBufferTester, AppendString)
{
  const std::string input = "0123456789Alaaaaaaaaaaaaaaaaaarm";

  SmartBuffer filebuffer;
  FileBuffer::ExpectVoid expectVoid;
  expectVoid = filebuffer.append(input);
  EXPECT_TRUE(expectVoid.has_value());
  if (!expectVoid.has_value()) {
    std::cout << expectVoid.error().what() << "\n";
    return;
  }

  std::string result;
  while (filebuffer.size() > 0) {
    FileBuffer::ExpectStr str = filebuffer.consumeFront(1);
    EXPECT_TRUE(str.has_value());
    if (!str.has_value()) {
      std::cout << str.error().what() << "\n";
      break;
    }
    result.append(*str);
  }
  EXPECT_EQ(result, input);
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
