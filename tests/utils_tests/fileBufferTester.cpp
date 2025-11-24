#include <utils/FileBuffer.hpp>

#include <gtest/gtest.h>
#include <string>

TEST(FileBufferTester, AppendString)
{
  const std::string input = "0123456789Alaaaaaaaaaaaaaaaaaarm";

  FileBuffer filebuffer;
  filebuffer.append(input);

  std::string result;
  while (filebuffer.size() > 0) {
    const std::string front = filebuffer.consumeFront(1);
    result.append(front);
  }
  EXPECT_EQ(result, input);
}

TEST(FileBufferTester, AppendVector)
{
  std::string inputStr = "0123456789Alaaaaaaaaaaaaaaaaaarm";
  const FileBuffer::RawBytes input(inputStr.begin(), inputStr.end());

  FileBuffer filebuffer;
  filebuffer.append(input, input.size());

  std::string result;
  while (filebuffer.size() > 0) {
    const std::string front = filebuffer.consumeFront(1);
    result.append(front);
  }
  EXPECT_EQ(result, inputStr);
}

TEST(FileBufferTester, GetPeekSeek)
{
  const std::string input = "0123456789";

  FileBuffer filebuffer;
  filebuffer.append(input);
  filebuffer.seek(4);

  EXPECT_EQ(filebuffer.get(), '4');
  EXPECT_EQ(filebuffer.get(), '5');
  EXPECT_EQ(filebuffer.peek(), '6');
  EXPECT_EQ(filebuffer.peek(), '6');
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
