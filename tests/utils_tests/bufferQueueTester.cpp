#include <libftpp/memory.hpp>
#include <utils/buffer/BufferQueue.hpp>
#include <utils/buffer/MemoryBuffer.hpp>
#include <utils/buffer/StaticFileBuffer.hpp>

#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>
#include <string>

TEST(BufferQueueTester, EmptyQueue)
{
  BufferQueue queue;
  EXPECT_TRUE(queue.isEmpty());
}

TEST(BufferQueueTester, AppendSingleBuffer)
{
  BufferQueue queue;

  ft::shared_ptr<MemoryBuffer> buffer = ft::make_shared<MemoryBuffer>();
  buffer->append("Hello World");

  queue.append(buffer);
  EXPECT_FALSE(queue.isEmpty());
}

TEST(BufferQueueTester, AppendMultipleBuffers)
{
  BufferQueue queue;

  ft::shared_ptr<MemoryBuffer> buffer1 = ft::make_shared<MemoryBuffer>();
  buffer1->append("First");

  ft::shared_ptr<MemoryBuffer> buffer2 = ft::make_shared<MemoryBuffer>();
  buffer2->append("Second");

  ft::shared_ptr<MemoryBuffer> buffer3 = ft::make_shared<MemoryBuffer>();
  buffer3->append("Third");

  queue.append(buffer1);
  queue.append(buffer2);
  queue.append(buffer3);

  EXPECT_FALSE(queue.isEmpty());
}

TEST(BufferQueueTester, GetSmartBuffer)
{
  BufferQueue queue;

  SmartBuffer& smartBuffer = queue.getSmartBuffer();
  smartBuffer.append("Test Data");

  EXPECT_EQ(smartBuffer.size(), 9);
  EXPECT_FALSE(queue.isEmpty());
}

TEST(BufferQueueTester, MixedBufferTypes)
{
  BufferQueue queue;

  // Create a temporary file for StaticFileBuffer
  std::string testFilePath;
  testFilePath.append(ASSETS_PATH);
  testFilePath.append("TestFile_BufferQueueTester.txt");

  {
    std::ofstream outFile(testFilePath.c_str());
    outFile << "File Content";
    outFile.close();
  }

  ft::shared_ptr<MemoryBuffer> memBuffer = ft::make_shared<MemoryBuffer>();
  memBuffer->append("Memory Content");

  ft::shared_ptr<StaticFileBuffer> fileBuffer =
    ft::make_shared<StaticFileBuffer>(testFilePath);

  queue.append(memBuffer);
  queue.append(fileBuffer);

  EXPECT_FALSE(queue.isEmpty());

  (void)std::remove(testFilePath.c_str());
}

TEST(BufferQueueTester, AppendEmptyBuffer)
{
  BufferQueue queue;

  ft::shared_ptr<MemoryBuffer> emptyBuffer = ft::make_shared<MemoryBuffer>();
  queue.append(emptyBuffer);

  // Queue might optimize away empty buffers or keep them
  // Just verify no crash occurs
  EXPECT_NO_THROW(queue.getSmartBuffer());
}

TEST(BufferQueueTester, SmartBufferAfterOtherBuffers)
{
  BufferQueue queue;

  ft::shared_ptr<MemoryBuffer> buffer = ft::make_shared<MemoryBuffer>();
  buffer->append("First Buffer");
  queue.append(buffer);

  SmartBuffer& smartBuffer = queue.getSmartBuffer();
  smartBuffer.append("Smart Buffer Content");

  EXPECT_FALSE(queue.isEmpty());
  EXPECT_EQ(smartBuffer.size(), 20);
}

TEST(BufferQueueTester, MultipleSmartBufferAccesses)
{
  BufferQueue queue;

  SmartBuffer& smartBuffer1 = queue.getSmartBuffer();
  smartBuffer1.append("First");

  SmartBuffer& smartBuffer2 = queue.getSmartBuffer();
  smartBuffer2.append("Second");

  // Should be the same buffer instance
  EXPECT_EQ(&smartBuffer1, &smartBuffer2);
  EXPECT_EQ(smartBuffer1.size(), 11);
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
