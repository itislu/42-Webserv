#include <utils/buffer/MemoryBuffer.hpp>

#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>
#include <string>

TEST(MemoryBufferTester, AppendString)
{
  const std::string input = "0123456789Alaaaaaaaaaaaaaaaaaarm";

  MemoryBuffer MemoryBuffer;
  MemoryBuffer.append(input);

  std::string result;
  while (MemoryBuffer.size() > 0) {
    const std::string front = MemoryBuffer.consumeFront(1);
    result.append(front);
  }
  EXPECT_EQ(result, input);
}

TEST(MemoryBufferTester, AppendVector)
{
  std::string inputStr = "0123456789Alaaaaaaaaaaaaaaaaaarm";
  const MemoryBuffer::RawBytes input(inputStr.begin(), inputStr.end());

  MemoryBuffer MemoryBuffer;
  MemoryBuffer.append(input, input.size());

  std::string result;
  while (MemoryBuffer.size() > 0) {
    const std::string front = MemoryBuffer.consumeFront(1);
    result.append(front);
  }
  EXPECT_EQ(result, inputStr);
}

TEST(MemoryBufferTester, GetPeekSeek)
{
  const std::string input = "0123456789";

  MemoryBuffer MemoryBuffer;
  MemoryBuffer.append(input);
  MemoryBuffer.seek(4);

  EXPECT_EQ(MemoryBuffer.get(), '4');
  EXPECT_EQ(MemoryBuffer.get(), '5');
  EXPECT_EQ(MemoryBuffer.peek(), '6');
  EXPECT_EQ(MemoryBuffer.peek(), '6');
}

TEST(MemoryBufferTester, MoveToFile)
{
  std::string testFilePath;
  testFilePath.append(ASSETS_PATH);
  testFilePath.append("TestFile_MemoryBufferTester.txt");

  MemoryBuffer MemoryBuffer;
  MemoryBuffer.append("HelloWorld");

  MemoryBuffer.moveBufferToFile(testFilePath);

  const std::fstream fstream(testFilePath);
  EXPECT_TRUE(fstream.is_open());
  EXPECT_EQ(MemoryBuffer.size(), 0);

  (void)std::remove(testFilePath.c_str());
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
