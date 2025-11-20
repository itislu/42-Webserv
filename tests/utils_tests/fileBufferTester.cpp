#include <libftpp/expected.hpp>
#include <utils/FileBuffer.hpp>

#include <gtest/gtest.h>
#include <iostream>
#include <string>

TEST(FileBufferTester, AppendString)
{
  const std::string input = "0123456789Alaaaaaaaaaaaaaaaaaarm";

  FileBuffer filebuffer;
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

TEST(FileBufferTester, AppendVector)
{
  std::string inputStr = "0123456789Alaaaaaaaaaaaaaaaaaarm";
  const FileBuffer::RawBytes input(inputStr.begin(), inputStr.end());

  FileBuffer filebuffer;
  FileBuffer::ExpectVoid expectVoid;

  expectVoid = filebuffer.append(input, static_cast<long>(input.size()));
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
  EXPECT_EQ(result, inputStr);
}

TEST(FileBufferTester, GetPeekSeek)
{
  const std::string input = "0123456789";

  FileBuffer filebuffer;
  FileBuffer::ExpectVoid expectVoid;

  expectVoid = filebuffer.append(input);
  EXPECT_TRUE(expectVoid.has_value());
  if (!expectVoid.has_value()) {
    std::cout << expectVoid.error().what() << "\n";
    return;
  }

  expectVoid = filebuffer.seek(4);
  EXPECT_TRUE(expectVoid.has_value());
  if (!expectVoid.has_value()) {
    std::cout << expectVoid.error().what() << "\n";
    return;
  }

  FileBuffer::ExpectChr expectChr;
  expectChr = filebuffer.get();
  EXPECT_TRUE(expectChr.has_value());
  if (expectChr.has_value()) {
    EXPECT_EQ(expectChr.value(), '4');
  }
  expectChr = filebuffer.get();
  EXPECT_TRUE(expectChr.has_value());
  if (expectChr.has_value()) {
    EXPECT_EQ(expectChr.value(), '5');
  }
  expectChr = filebuffer.peek();
  EXPECT_TRUE(expectChr.has_value());
  if (expectChr.has_value()) {
    EXPECT_EQ(expectChr.value(), '6');
  }
  expectChr = filebuffer.peek();
  EXPECT_TRUE(expectChr.has_value());
  if (expectChr.has_value()) {
    EXPECT_EQ(expectChr.value(), '6');
  }
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
