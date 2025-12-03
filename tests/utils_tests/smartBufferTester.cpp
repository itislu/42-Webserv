#include <libftpp/utility.hpp>
#include <utils/buffer/FileBuffer.hpp>
#include <utils/buffer/MemoryBuffer.hpp>
#include <utils/buffer/SmartBuffer.hpp>

#include <gtest/gtest.h>
#include <string>

class SmartBufferTest : public SmartBuffer
{
public:
  using SmartBuffer::getRawBuffer;
};

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

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
