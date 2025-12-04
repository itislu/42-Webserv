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

TEST(FileBufferTester, AppendEmptyString)
{
  FileBuffer filebuffer;
  filebuffer.append("");
  EXPECT_EQ(filebuffer.size(), 0);
  EXPECT_TRUE(filebuffer.isEmpty());
}

TEST(FileBufferTester, AppendMultipleTimes)
{
  FileBuffer filebuffer;
  filebuffer.append("Hello");
  filebuffer.append(" ");
  filebuffer.append("World");

  const std::size_t expectedSize = 11;
  EXPECT_EQ(filebuffer.size(), expectedSize);
  const std::string result = filebuffer.consumeFront(expectedSize);
  EXPECT_EQ(result, "Hello World");
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

TEST(FileBufferTester, ConsumeFrontMultipleBytes)
{
  FileBuffer filebuffer;
  filebuffer.append("0123456789ABCDEF");

  const std::string first = filebuffer.consumeFront(5);
  EXPECT_EQ(first, "01234");
  EXPECT_EQ(filebuffer.size(), 11);

  const std::string second = filebuffer.consumeFront(3);
  EXPECT_EQ(second, "567");
  EXPECT_EQ(filebuffer.size(), 8);

  const std::string remaining = filebuffer.consumeFront(8);
  EXPECT_EQ(remaining, "89ABCDEF");
  EXPECT_TRUE(filebuffer.isEmpty());
}

TEST(FileBufferTester, GetStrMethod)
{
  FileBuffer filebuffer;
  filebuffer.append("Hello World!");

  const std::string substr1 = filebuffer.getStr(0, 5);
  EXPECT_EQ(substr1, "Hello");
  EXPECT_EQ(filebuffer.size(), 12); // Size should remain unchanged

  const std::string substr2 = filebuffer.getStr(6, 5);
  EXPECT_EQ(substr2, "World");
  EXPECT_EQ(filebuffer.size(), 12); // Size should remain unchanged
}

TEST(FileBufferTester, RemoveFront)
{
  FileBuffer filebuffer;
  filebuffer.append("0123456789");

  filebuffer.removeFront(3);
  EXPECT_EQ(filebuffer.size(), 7);

  const std::string remaining = filebuffer.consumeFront(7);
  EXPECT_EQ(remaining, "3456789");
}

TEST(FileBufferTester, ReplaceBuffer)
{
  FileBuffer filebuffer;
  filebuffer.append("Original Content");

  FileBuffer::RawBytes newData;
  const std::string newStr = "New Content";
  newData.assign(newStr.begin(), newStr.end());

  filebuffer.replace(newData);
  EXPECT_EQ(filebuffer.size(), newStr.size());

  const std::string result = filebuffer.consumeFront(filebuffer.size());
  EXPECT_EQ(result, "New Content");
}

TEST(FileBufferTester, ConsumeAll)
{
  FileBuffer filebuffer;
  const std::string input = "Test Data for ConsumeAll";
  filebuffer.append(input);

  const FileBuffer::RawBytes allData = filebuffer.consumeAll();
  EXPECT_TRUE(filebuffer.isEmpty());
  EXPECT_EQ(filebuffer.size(), 0);

  const std::string result(allData.begin(), allData.end());
  EXPECT_EQ(result, input);
}

TEST(FileBufferTester, SeekAndPos)
{
  FileBuffer filebuffer;
  filebuffer.append("0123456789");

  EXPECT_EQ(filebuffer.pos(), 0);

  filebuffer.seek(5);
  EXPECT_EQ(filebuffer.pos(), 5);
  EXPECT_EQ(filebuffer.get(), '5');

  filebuffer.seek(0);
  EXPECT_EQ(filebuffer.pos(), 0);
  EXPECT_EQ(filebuffer.peek(), '0');
}

TEST(FileBufferTester, LargeData)
{
  FileBuffer filebuffer;
  std::string largeData(10000, 'X');

  filebuffer.append(largeData);
  EXPECT_EQ(filebuffer.size(), 10000);

  const std::string result = filebuffer.consumeFront(10000);
  EXPECT_EQ(result.size(), 10000);
  EXPECT_TRUE(filebuffer.isEmpty());
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

TEST(StaticFileBufferTester, ReadCompleteFile)
{
  std::string testFilePath;
  testFilePath.append(ASSETS_PATH);
  testFilePath.append("TestFile_StaticFileBuffer1.txt");

  try {
    // Create test file
    {
      std::ofstream outFile(testFilePath.c_str());
      outFile << "Complete File Content";
      outFile.close();
    }

    StaticFileBuffer sfb(testFilePath);
    EXPECT_EQ(sfb.size(), 21);
    EXPECT_FALSE(sfb.isEmpty());

    const std::string content = sfb.consumeFront(sfb.size());
    EXPECT_EQ(content, "Complete File Content");
    EXPECT_TRUE(sfb.isEmpty());

    (void)std::remove(testFilePath.c_str());
  } catch (const std::exception& e) {
    (void)std::remove(testFilePath.c_str());
    FAIL() << e.what() << '\n';
  }
}

TEST(StaticFileBufferTester, GetAndPeek)
{
  std::string testFilePath;
  testFilePath.append(ASSETS_PATH);
  testFilePath.append("TestFile_StaticFileBuffer2.txt");

  try {
    {
      std::ofstream outFile(testFilePath.c_str());
      outFile << "0123456789";
      outFile.close();
    }

    StaticFileBuffer sfb(testFilePath);

    EXPECT_EQ(sfb.peek(), '0');
    EXPECT_EQ(sfb.peek(), '0');
    EXPECT_EQ(sfb.get(), '0');
    EXPECT_EQ(sfb.get(), '1');
    EXPECT_EQ(sfb.peek(), '2');

    (void)std::remove(testFilePath.c_str());
  } catch (const std::exception& e) {
    (void)std::remove(testFilePath.c_str());
    FAIL() << e.what() << '\n';
  }
}

TEST(StaticFileBufferTester, SeekOperations)
{
  std::string testFilePath;
  testFilePath.append(ASSETS_PATH);
  testFilePath.append("TestFile_StaticFileBuffer3.txt");

  try {
    {
      std::ofstream outFile(testFilePath.c_str());
      outFile << "ABCDEFGHIJ";
      outFile.close();
    }

    StaticFileBuffer sfb(testFilePath);

    EXPECT_EQ(sfb.pos(), 0);

    const std::size_t seekPos = 5;
    sfb.seek(seekPos);
    EXPECT_EQ(sfb.pos(), seekPos);
    EXPECT_EQ(sfb.peek(), 'F');

    sfb.seek(0);
    EXPECT_EQ(sfb.pos(), 0);
    EXPECT_EQ(sfb.peek(), 'A');

    sfb.seek(9);
    EXPECT_EQ(sfb.peek(), 'J');

    (void)std::remove(testFilePath.c_str());
  } catch (const std::exception& e) {
    (void)std::remove(testFilePath.c_str());
    FAIL() << e.what() << '\n';
  }
}

TEST(StaticFileBufferTester, GetStrMethod)
{
  std::string testFilePath;
  testFilePath.append(ASSETS_PATH);
  testFilePath.append("TestFile_StaticFileBuffer4.txt");

  try {
    {
      std::ofstream outFile(testFilePath.c_str());
      outFile << "Hello Beautiful World";
      outFile.close();
    }

    StaticFileBuffer sfb(testFilePath);

    const std::string sub1 = sfb.getStr(0, 5);
    EXPECT_EQ(sub1, "Hello");

    const std::string sub2 = sfb.getStr(6, 9);
    EXPECT_EQ(sub2, "Beautiful");

    const std::string sub3 = sfb.getStr(16, 5);
    EXPECT_EQ(sub3, "World");

    EXPECT_EQ(sfb.size(), 21); // Size unchanged

    (void)std::remove(testFilePath.c_str());
  } catch (const std::exception& e) {
    (void)std::remove(testFilePath.c_str());
    FAIL() << e.what() << '\n';
  }
}

TEST(StaticFileBufferTester, ConsumeRawFront)
{
  std::string testFilePath;
  testFilePath.append(ASSETS_PATH);
  testFilePath.append("TestFile_StaticFileBuffer5.txt");

  try {
    {
      std::ofstream outFile(testFilePath.c_str());
      outFile << "RawBytesTest";
      outFile.close();
    }

    StaticFileBuffer sfb(testFilePath);

    const StaticFileBuffer::RawBytes raw = sfb.consumeRawFront(8);
    const std::string result(raw.begin(), raw.end());
    EXPECT_EQ(result, "RawBytes");
    EXPECT_EQ(sfb.size(), 4);

    (void)std::remove(testFilePath.c_str());
  } catch (const std::exception& e) {
    (void)std::remove(testFilePath.c_str());
    FAIL() << e.what() << '\n';
  }
}

TEST(StaticFileBufferTester, GetRawBytes)
{
  std::string testFilePath;
  testFilePath.append(ASSETS_PATH);
  testFilePath.append("TestFile_StaticFileBuffer6.txt");

  try {
    {
      std::ofstream outFile(testFilePath.c_str());
      outFile << "Test Raw Access";
      outFile.close();
    }

    StaticFileBuffer sfb(testFilePath);

    const StaticFileBuffer::RawBytes raw = sfb.getRawBytes(5, 3);
    const std::string result(raw.begin(), raw.end());
    EXPECT_EQ(result, "Raw");
    EXPECT_EQ(sfb.size(), 15); // Size unchanged

    (void)std::remove(testFilePath.c_str());
  } catch (const std::exception& e) {
    (void)std::remove(testFilePath.c_str());
    FAIL() << e.what() << '\n';
  }
}

TEST(StaticFileBufferTester, ConsumeAll)
{
  std::string testFilePath;
  testFilePath.append(ASSETS_PATH);
  testFilePath.append("TestFile_StaticFileBuffer7.txt");

  try {
    const std::string content = "Consume Everything";
    {
      std::ofstream outFile(testFilePath.c_str());
      outFile << content;
      outFile.close();
    }

    StaticFileBuffer sfb(testFilePath);

    const StaticFileBuffer::RawBytes all = sfb.consumeAll();
    const std::string result(all.begin(), all.end());
    EXPECT_EQ(result, content);
    EXPECT_TRUE(sfb.isEmpty());
    EXPECT_EQ(sfb.size(), 0);

    (void)std::remove(testFilePath.c_str());
  } catch (const std::exception& e) {
    (void)std::remove(testFilePath.c_str());
    FAIL() << e.what() << '\n';
  }
}

TEST(StaticFileBufferTester, LargeFile)
{
  std::string testFilePath;
  testFilePath.append(ASSETS_PATH);
  testFilePath.append("TestFile_StaticFileBuffer8.txt");

  try {
    const std::size_t largeSize = 50000;
    {
      std::ofstream outFile(testFilePath.c_str());
      for (std::size_t i = 0; i < largeSize; ++i) {
        outFile << 'X';
      }
      outFile.close();
    }

    StaticFileBuffer sfb(testFilePath);
    EXPECT_EQ(sfb.size(), largeSize);

    const std::string partial = sfb.consumeFront(1000);
    EXPECT_EQ(partial.size(), 1000);
    EXPECT_EQ(sfb.size(), largeSize - 1000);

    (void)std::remove(testFilePath.c_str());
  } catch (const std::exception& e) {
    (void)std::remove(testFilePath.c_str());
    FAIL() << e.what() << '\n';
  }
}

TEST(StaticFileBufferTester, EmptyFile)
{
  std::string testFilePath;
  testFilePath.append(ASSETS_PATH);
  testFilePath.append("TestFile_StaticFileBuffer9.txt");

  try {
    {
      std::ofstream outFile(testFilePath.c_str());
      outFile.close();
    }

    StaticFileBuffer sfb(testFilePath);
    EXPECT_TRUE(sfb.isEmpty());
    EXPECT_EQ(sfb.size(), 0);

    (void)std::remove(testFilePath.c_str());
  } catch (const std::exception& e) {
    (void)std::remove(testFilePath.c_str());
    FAIL() << e.what() << '\n';
  }
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
