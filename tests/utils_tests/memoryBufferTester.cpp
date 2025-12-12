#include <utils/buffer/MemoryBuffer.hpp>

#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>
#include <string>

TEST(MemoryBufferTester, AppendString)
{
  const std::string input = "0123456789Alaaaaaaaaaaaaaaaaaarm";

  MemoryBuffer memoryBuffer;
  memoryBuffer.append(input);

  std::string result;
  while (memoryBuffer.size() > 0) {
    const std::string front = memoryBuffer.consumeFront(1);
    result.append(front);
  }
  EXPECT_EQ(result, input);
}

TEST(MemoryBufferTester, AppendVector)
{
  std::string inputStr = "0123456789Alaaaaaaaaaaaaaaaaaarm";
  const MemoryBuffer::RawBytes input(inputStr.begin(), inputStr.end());

  MemoryBuffer memoryBuffer;
  memoryBuffer.append(input, input.size());

  std::string result;
  while (memoryBuffer.size() > 0) {
    const std::string front = memoryBuffer.consumeFront(1);
    result.append(front);
  }
  EXPECT_EQ(result, inputStr);
}

TEST(MemoryBufferTester, GetPeekSeek)
{
  const std::string input = "0123456789";

  MemoryBuffer memoryBuffer;
  memoryBuffer.append(input);
  memoryBuffer.seek(4);

  EXPECT_EQ(memoryBuffer.get(), '4');
  EXPECT_EQ(memoryBuffer.get(), '5');
  EXPECT_EQ(memoryBuffer.peek(), '6');
  EXPECT_EQ(memoryBuffer.peek(), '6');
}

TEST(MemoryBufferTester, MoveToFile)
{
  std::string testFilePath;
  testFilePath.append(ASSETS_PATH);
  testFilePath.append("TestFile_MemoryBufferTester.txt");

  MemoryBuffer memoryBuffer;
  memoryBuffer.append("HelloWorld");

  memoryBuffer.moveBufferToFile(testFilePath);

  const std::fstream fstream(testFilePath);
  EXPECT_TRUE(fstream.is_open());
  EXPECT_EQ(memoryBuffer.size(), 0);

  (void)std::remove(testFilePath.c_str());
}

TEST(MemoryBufferTester, Reset)
{
  MemoryBuffer memoryBuffer;
  memoryBuffer.append("HelloWorld");
  memoryBuffer.seek(4);

  EXPECT_EQ(memoryBuffer.size(), 10);
  EXPECT_EQ(memoryBuffer.pos(), 4);

  memoryBuffer.reset();

  EXPECT_EQ(memoryBuffer.size(), 0);
  EXPECT_TRUE(memoryBuffer.isEmpty());
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
