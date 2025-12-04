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
  const std::string inputStr = "0123456789Alaaaaaaaaaaaaaaaaaarm";
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
  EXPECT_EQ(memoryBuffer.get(), '6');

  memoryBuffer.seek(0);
  EXPECT_EQ(memoryBuffer.get(), '0');
  EXPECT_EQ(memoryBuffer.peek(), '1');
  EXPECT_EQ(memoryBuffer.peek(), '1');
  EXPECT_EQ(memoryBuffer.get(), '1');
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

TEST(MemoryBufferTester, AppendEmptyString)
{
  MemoryBuffer memoryBuffer;
  memoryBuffer.append("");
  EXPECT_EQ(memoryBuffer.size(), 0);
  EXPECT_TRUE(memoryBuffer.isEmpty());
}

TEST(MemoryBufferTester, AppendMultipleTimes)
{
  MemoryBuffer memoryBuffer;
  memoryBuffer.append("First");
  memoryBuffer.append("-");
  memoryBuffer.append("Second");

  const std::size_t expectedSize = 12;
  EXPECT_EQ(memoryBuffer.size(), expectedSize);
  const std::string result = memoryBuffer.consumeFront(expectedSize);
  EXPECT_EQ(result, "First-Second");
}

TEST(MemoryBufferTester, ConsumeFrontMultipleBytes)
{
  MemoryBuffer memoryBuffer;
  memoryBuffer.append("ABCDEFGHIJKLMNOP");

  const std::string first = memoryBuffer.consumeFront(4);
  EXPECT_EQ(first, "ABCD");
  EXPECT_EQ(memoryBuffer.size(), 12);

  const std::string second = memoryBuffer.consumeFront(4);
  EXPECT_EQ(second, "EFGH");
  EXPECT_EQ(memoryBuffer.size(), 8);

  const std::string remaining = memoryBuffer.consumeFront(8);
  EXPECT_EQ(remaining, "IJKLMNOP");
  EXPECT_TRUE(memoryBuffer.isEmpty());
}

TEST(MemoryBufferTester, GetStrMethod)
{
  MemoryBuffer memoryBuffer;
  memoryBuffer.append("Hello Beautiful World!");

  const std::string substr1 = memoryBuffer.getStr(0, 5);
  EXPECT_EQ(substr1, "Hello");
  EXPECT_EQ(memoryBuffer.size(), 22); // Size should remain unchanged

  const std::string substr2 = memoryBuffer.getStr(6, 9);
  EXPECT_EQ(substr2, "Beautiful");
  EXPECT_EQ(memoryBuffer.size(), 22); // Size should remain unchanged

  const std::string substr3 = memoryBuffer.getStr(16, 6);
  EXPECT_EQ(substr3, "World!");
}

TEST(MemoryBufferTester, GetRawBytesMethod)
{
  MemoryBuffer memoryBuffer;
  memoryBuffer.append("Test Raw Bytes");

  const MemoryBuffer::RawBytes rawBytes = memoryBuffer.getRawBytes(5, 3);
  const std::string result(rawBytes.begin(), rawBytes.end());
  EXPECT_EQ(result, "Raw");
  EXPECT_EQ(memoryBuffer.size(), 14); // Size unchanged
}

TEST(MemoryBufferTester, RemoveFront)
{
  MemoryBuffer memoryBuffer;
  memoryBuffer.append("0123456789ABCDEF");

  memoryBuffer.removeFront(5);
  EXPECT_EQ(memoryBuffer.size(), 11);

  const std::string result = memoryBuffer.consumeFront(11);
  EXPECT_EQ(result, "56789ABCDEF");
}

TEST(MemoryBufferTester, ReplaceBuffer)
{
  MemoryBuffer memoryBuffer;
  memoryBuffer.append("Old Data");

  MemoryBuffer::RawBytes newData;
  const std::string newStr = "Replaced";
  newData.assign(newStr.begin(), newStr.end());

  memoryBuffer.replace(newData);
  EXPECT_EQ(memoryBuffer.size(), newStr.size());

  const std::string result = memoryBuffer.consumeFront(memoryBuffer.size());
  EXPECT_EQ(result, "Replaced");
}

TEST(MemoryBufferTester, ConsumeAll)
{
  MemoryBuffer memoryBuffer;
  const std::string input = "Everything Should Be Consumed";
  memoryBuffer.append(input);

  const MemoryBuffer::RawBytes allData = memoryBuffer.consumeAll();
  EXPECT_TRUE(memoryBuffer.isEmpty());
  EXPECT_EQ(memoryBuffer.size(), 0);

  const std::string result(allData.begin(), allData.end());
  EXPECT_EQ(result, input);
}

TEST(MemoryBufferTester, SeekAndPos)
{
  MemoryBuffer memoryBuffer;
  memoryBuffer.append("0123456789ABCDEF");

  EXPECT_EQ(memoryBuffer.pos(), 0);

  const std::size_t seekPos = 10;
  memoryBuffer.seek(seekPos);
  EXPECT_EQ(memoryBuffer.pos(), seekPos);
  EXPECT_EQ(memoryBuffer.get(), 'A');

  memoryBuffer.seek(0);
  EXPECT_EQ(memoryBuffer.pos(), 0);
  EXPECT_EQ(memoryBuffer.peek(), '0');
}

TEST(MemoryBufferTester, ConsumeRawFront)
{
  MemoryBuffer memoryBuffer;
  memoryBuffer.append("RawByteData");

  const MemoryBuffer::RawBytes rawData = memoryBuffer.consumeRawFront(7);
  const std::string result(rawData.begin(), rawData.end());
  EXPECT_EQ(result, "RawByte");
  EXPECT_EQ(memoryBuffer.size(), 4);

  const std::string remaining = memoryBuffer.consumeFront(4);
  EXPECT_EQ(remaining, "Data");
}

TEST(MemoryBufferTester, LargeData)
{
  MemoryBuffer memoryBuffer;
  const std::size_t largeSize = 100000;
  const std::string largeData(largeSize, 'Z');

  memoryBuffer.append(largeData);
  EXPECT_EQ(memoryBuffer.size(), largeSize);

  const std::string result = memoryBuffer.consumeFront(largeSize);
  EXPECT_EQ(result.size(), largeSize);
  EXPECT_TRUE(memoryBuffer.isEmpty());
}

TEST(MemoryBufferTester, ConstructorWithString)
{
  const std::string input = "Constructor Test";
  MemoryBuffer memoryBuffer(input);

  EXPECT_EQ(memoryBuffer.size(), input.size());
  const std::string result = memoryBuffer.consumeFront(memoryBuffer.size());
  EXPECT_EQ(result, input);
}

TEST(MemoryBufferTester, ConstructorWithRawBytes)
{
  const std::string inputStr = "RawBytes Constructor";
  const MemoryBuffer::RawBytes input(inputStr.begin(), inputStr.end());
  MemoryBuffer memoryBuffer(input, input.size());

  EXPECT_EQ(memoryBuffer.size(), input.size());
  const std::string result = memoryBuffer.consumeFront(memoryBuffer.size());
  EXPECT_EQ(result, inputStr);
}

TEST(MemoryBufferTester, MultipleSeeks)
{
  MemoryBuffer memoryBuffer;
  memoryBuffer.append("0123456789");

  memoryBuffer.seek(5);
  EXPECT_EQ(memoryBuffer.peek(), '5');

  memoryBuffer.seek(2);
  EXPECT_EQ(memoryBuffer.peek(), '2');

  memoryBuffer.seek(8);
  EXPECT_EQ(memoryBuffer.peek(), '8');

  memoryBuffer.seek(0);
  EXPECT_EQ(memoryBuffer.peek(), '0');
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
