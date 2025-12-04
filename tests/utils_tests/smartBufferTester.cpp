#include <libftpp/utility.hpp>
#include <utils/buffer/FileBuffer.hpp>
#include <utils/buffer/MemoryBuffer.hpp>
#include <utils/buffer/SmartBuffer.hpp>

#include <gtest/gtest.h>
#include <string>

namespace {

class SmartBufferTest : public SmartBuffer
{
public:
  using SmartBuffer::getRawBuffer;
};

}

// NOLINTBEGIN(readability-magic-numbers)
TEST(SmartBufferTester, AppendString)
{
  SmartBufferTest::setMemoryToFileThreshold(11);
  SmartBufferTest::setFileToMemoryThreshold(3);

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
// NOLINTEND(readability-magic-numbers)

// NOLINTBEGIN(readability-magic-numbers)
TEST(SmartBufferTester, SwitchToFile)
{
  SmartBufferTest::setMemoryToFileThreshold(11);
  SmartBufferTest::setFileToMemoryThreshold(3);
  SmartBufferTest smartBuffer;

  smartBuffer.append(std::string("0123456789"));

  // should still be memory buffer
  {
    const MemoryBuffer* const memPtr =
      dynamic_cast<const MemoryBuffer*>(smartBuffer.getRawBuffer());
    EXPECT_NE(memPtr, FT_NULLPTR);
    EXPECT_EQ(smartBuffer.size(), 10);
  }

  smartBuffer.append(std::string("0"));

  // should be file buffer
  {
    const FileBuffer* const filePtr =
      dynamic_cast<const FileBuffer*>(smartBuffer.getRawBuffer());
    EXPECT_NE(filePtr, FT_NULLPTR);
    EXPECT_EQ(smartBuffer.size(), 11);
  }

  smartBuffer.removeFront(7);

  // should still be file buffer
  {
    const FileBuffer* const filePtr =
      dynamic_cast<const FileBuffer*>(smartBuffer.getRawBuffer());
    EXPECT_NE(filePtr, FT_NULLPTR);
    EXPECT_EQ(smartBuffer.size(), 4);
  }

  smartBuffer.removeFront(1);

  // should be memory buffer
  {
    const MemoryBuffer* const memPtr =
      dynamic_cast<const MemoryBuffer*>(smartBuffer.getRawBuffer());
    EXPECT_NE(memPtr, FT_NULLPTR);
    EXPECT_EQ(smartBuffer.size(), 3);
  }
}
// NOLINTEND(readability-magic-numbers)

// NOLINTBEGIN(readability-magic-numbers)
TEST(SmartBufferTester, EmptyBuffer)
{
  SmartBuffer buffer;
  EXPECT_TRUE(buffer.isEmpty());
  EXPECT_EQ(buffer.size(), 0);
}
// NOLINTEND(readability-magic-numbers)

// NOLINTBEGIN(readability-magic-numbers)
TEST(SmartBufferTester, GetPeekSeek)
{
  SmartBufferTest::setMemoryToFileThreshold(100);
  SmartBufferTest::setFileToMemoryThreshold(10);

  SmartBuffer buffer;
  buffer.append("0123456789ABCDEF");

  EXPECT_EQ(buffer.pos(), 0);
  buffer.seek(5);
  EXPECT_EQ(buffer.pos(), 5);
  EXPECT_EQ(buffer.peek(), '5');
  EXPECT_EQ(buffer.get(), '5');
  EXPECT_EQ(buffer.get(), '6');

  buffer.seek(0);
  EXPECT_EQ(buffer.pos(), 0);
  EXPECT_EQ(buffer.peek(), '0');
}
// NOLINTEND(readability-magic-numbers)

// NOLINTBEGIN(readability-magic-numbers)
TEST(SmartBufferTester, ConsumeFront)
{
  SmartBufferTest::setMemoryToFileThreshold(100);
  SmartBufferTest::setFileToMemoryThreshold(10);

  SmartBuffer buffer;
  buffer.append("HelloWorld");

  const std::string first = buffer.consumeFront(5);
  EXPECT_EQ(first, "Hello");
  EXPECT_EQ(buffer.size(), 5);

  const std::string second = buffer.consumeFront(5);
  EXPECT_EQ(second, "World");
  EXPECT_TRUE(buffer.isEmpty());
}
// NOLINTEND(readability-magic-numbers)

// NOLINTBEGIN(readability-magic-numbers)
TEST(SmartBufferTester, GetStrMethod)
{
  SmartBufferTest::setMemoryToFileThreshold(100);
  SmartBufferTest::setFileToMemoryThreshold(10);

  SmartBuffer buffer;
  buffer.append("Testing GetStr Method");

  const std::string substr1 = buffer.getStr(0, 7);
  EXPECT_EQ(substr1, "Testing");
  EXPECT_EQ(buffer.size(), 21); // Size unchanged

  const std::string substr2 = buffer.getStr(8, 6);
  EXPECT_EQ(substr2, "GetStr");
}
// NOLINTEND(readability-magic-numbers)

// NOLINTBEGIN(readability-magic-numbers)
TEST(SmartBufferTester, RemoveFront)
{
  SmartBufferTest::setMemoryToFileThreshold(100);
  SmartBufferTest::setFileToMemoryThreshold(10);

  SmartBuffer buffer;
  buffer.append("0123456789ABCDEF");

  buffer.removeFront(5);
  EXPECT_EQ(buffer.size(), 11);

  const std::string remaining = buffer.consumeFront(11);
  EXPECT_EQ(remaining, "56789ABCDEF");
}
// NOLINTEND(readability-magic-numbers)

// NOLINTBEGIN(readability-magic-numbers)
TEST(SmartBufferTester, ConsumeAll)
{
  SmartBufferTest::setMemoryToFileThreshold(100);
  SmartBufferTest::setFileToMemoryThreshold(10);

  SmartBuffer buffer;
  const std::string input = "Consume All Data";
  buffer.append(input);

  const SmartBuffer::RawBytes allData = buffer.consumeAll();
  EXPECT_TRUE(buffer.isEmpty());

  const std::string result(allData.begin(), allData.end());
  EXPECT_EQ(result, input);
}
// NOLINTEND(readability-magic-numbers)

// NOLINTBEGIN(readability-magic-numbers)
TEST(SmartBufferTester, ReplaceBuffer)
{
  SmartBufferTest::setMemoryToFileThreshold(100);
  SmartBufferTest::setFileToMemoryThreshold(10);

  SmartBuffer buffer;
  buffer.append("Original");

  SmartBuffer::RawBytes newData;
  const std::string newStr = "Replaced";
  newData.assign(newStr.begin(), newStr.end());

  buffer.replace(newData);
  EXPECT_EQ(buffer.size(), newStr.size());

  const std::string result = buffer.consumeFront(buffer.size());
  EXPECT_EQ(result, "Replaced");
}
// NOLINTEND(readability-magic-numbers)

// NOLINTBEGIN(readability-magic-numbers)
TEST(SmartBufferTester, SwitchBackAndForth)
{
  SmartBufferTest::setMemoryToFileThreshold(20);
  SmartBufferTest::setFileToMemoryThreshold(5);
  SmartBufferTest smartBuffer;

  // Start with memory
  smartBuffer.append("0123456789"); // 10 bytes
  {
    const MemoryBuffer* const memPtr =
      dynamic_cast<const MemoryBuffer*>(smartBuffer.getRawBuffer());
    EXPECT_NE(memPtr, FT_NULLPTR);
  }

  // Switch to file
  smartBuffer.append("0123456789"); // 20 bytes total
  {
    const FileBuffer* const filePtr =
      dynamic_cast<const FileBuffer*>(smartBuffer.getRawBuffer());
    EXPECT_NE(filePtr, FT_NULLPTR);
  }

  // Remove some data, should stay as file
  smartBuffer.removeFront(10); // 10 bytes remaining
  {
    const FileBuffer* const filePtr =
      dynamic_cast<const FileBuffer*>(smartBuffer.getRawBuffer());
    EXPECT_NE(filePtr, FT_NULLPTR);
  }

  // Remove more, should switch back to memory
  smartBuffer.removeFront(5); // 5 bytes remaining
  {
    const MemoryBuffer* const memPtr =
      dynamic_cast<const MemoryBuffer*>(smartBuffer.getRawBuffer());
    EXPECT_NE(memPtr, FT_NULLPTR);
  }
}
// NOLINTEND(readability-magic-numbers)

// NOLINTBEGIN(readability-magic-numbers)
TEST(SmartBufferTester, LargeDataHandling)
{
  SmartBufferTest::setMemoryToFileThreshold(1000);
  SmartBufferTest::setFileToMemoryThreshold(100);
  SmartBufferTest smartBuffer;

  // Add large data that should trigger file buffer
  const std::string largeData(2000, 'X');
  smartBuffer.append(largeData);

  {
    const FileBuffer* const filePtr =
      dynamic_cast<const FileBuffer*>(smartBuffer.getRawBuffer());
    EXPECT_NE(filePtr, FT_NULLPTR);
    EXPECT_EQ(smartBuffer.size(), 2000);
  }

  // Consume most of it
  smartBuffer.consumeFront(1899);

  {
    const FileBuffer* const filePtr =
      dynamic_cast<const FileBuffer*>(smartBuffer.getRawBuffer());
    EXPECT_NE(filePtr, FT_NULLPTR);
    EXPECT_EQ(smartBuffer.size(), 101);
  }

  // Remove to trigger switch back to memory
  smartBuffer.removeFront(1);

  {
    const MemoryBuffer* const memPtr =
      dynamic_cast<const MemoryBuffer*>(smartBuffer.getRawBuffer());
    EXPECT_NE(memPtr, FT_NULLPTR);
    EXPECT_EQ(smartBuffer.size(), 100);
  }
}
// NOLINTEND(readability-magic-numbers)

// NOLINTBEGIN(readability-magic-numbers)
TEST(SmartBufferTester, MoveToFile)
{
  SmartBufferTest::setMemoryToFileThreshold(100);
  SmartBufferTest::setFileToMemoryThreshold(10);

  SmartBuffer buffer;
  buffer.append("Data to move to file");

  std::string testFilePath;
  testFilePath.append(ASSETS_PATH);
  testFilePath.append("TestFile_SmartBufferTester.txt");

  buffer.moveBufferToFile(testFilePath);

  std::fstream fstream(testFilePath);
  EXPECT_TRUE(fstream.is_open());
  EXPECT_EQ(buffer.size(), 0);
  EXPECT_TRUE(buffer.isEmpty());

  (void)std::remove(testFilePath.c_str());
}
// NOLINTEND(readability-magic-numbers)

// NOLINTBEGIN(readability-magic-numbers)
TEST(SmartBufferTester, GetRawBytes)
{
  SmartBufferTest::setMemoryToFileThreshold(100);
  SmartBufferTest::setFileToMemoryThreshold(10);

  SmartBuffer buffer;
  buffer.append("Test Raw Bytes Access");

  const SmartBuffer::RawBytes rawBytes = buffer.getRawBytes(5, 3);
  const std::string result(rawBytes.begin(), rawBytes.end());
  EXPECT_EQ(result, "Raw");
  EXPECT_EQ(buffer.size(), 21); // Size unchanged
}
// NOLINTEND(readability-magic-numbers)

// NOLINTBEGIN(readability-magic-numbers)
TEST(SmartBufferTester, ConsumeRawFront)
{
  SmartBufferTest::setMemoryToFileThreshold(100);
  SmartBufferTest::setFileToMemoryThreshold(10);

  SmartBuffer buffer;
  buffer.append("RawFrontData");

  const SmartBuffer::RawBytes rawData = buffer.consumeRawFront(7);
  const std::string result(rawData.begin(), rawData.end());
  EXPECT_EQ(result, "RawFron");
  EXPECT_EQ(buffer.size(), 5);
}
// NOLINTEND(readability-magic-numbers)

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
