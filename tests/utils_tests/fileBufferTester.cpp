#include <utils/buffer/FileBuffer.hpp>
#include <utils/buffer/StaticFileBuffer.hpp>

#include <cstdio>
#include <exception>
#include <fstream>
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

TEST(FileBufferTester, MoveToFile)
{
  std::string testFilePath;
  testFilePath.append(ASSETS_PATH);
  testFilePath.append("TestFile_FileBufferTester.txt");

  FileBuffer fileBuffer;
  fileBuffer.append("HelloWorld");

  fileBuffer.moveBufferToFile(testFilePath);

  const std::fstream fstream(testFilePath);
  EXPECT_TRUE(fstream.is_open());
  EXPECT_EQ(fileBuffer.size(), 0);

  (void)std::remove(testFilePath.c_str());
}

TEST(FileBufferTester, StaticFileBuffer)
{
  std::string testFilePath;
  try {
    testFilePath.append(ASSETS_PATH);
    testFilePath.append("TestFile_FileBufferTester.txt");
    FileBuffer fileBuffer;
    fileBuffer.append("HelloWorld");
    fileBuffer.moveBufferToFile(testFilePath);

    StaticFileBuffer sfb(testFilePath);
    sfb.consumeRawFront(sfb.size() / 2);

    sfb.seek(3);
    EXPECT_EQ(sfb.pos(), 3);
    sfb.seek(0);

    sfb.consumeRawFront(sfb.size());
    EXPECT_TRUE(sfb.isEmpty());
    EXPECT_EQ(sfb.size(), 0);
    (void)std::remove(testFilePath.c_str());
  } catch (const std::exception& e) {
    if (!testFilePath.empty()) {
      (void)std::remove(testFilePath.c_str());
    }
    FAIL() << e.what() << '\n';
  }
}

TEST(FileBufferTester, Reset)
{
  FileBuffer fileBuffer;
  fileBuffer.append("HelloWorld");
  fileBuffer.seek(4);

  EXPECT_EQ(fileBuffer.size(), 10);
  EXPECT_EQ(fileBuffer.pos(), 4);

  fileBuffer.reset();

  EXPECT_EQ(fileBuffer.size(), 0);
  EXPECT_TRUE(fileBuffer.isEmpty());
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
