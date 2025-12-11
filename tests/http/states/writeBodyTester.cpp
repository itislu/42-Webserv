#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Response.hpp>
#include <http/http.hpp>
#include <http/states/writeBody/WriteBody.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/string.hpp>
#include <utils/buffer/MemoryBuffer.hpp>
#include <utils/buffer/SmartBuffer.hpp>

#include <algorithm>
#include <cstddef>
#include <gtest/gtest.h>
#include <ios>
#include <string>

namespace {

void setupStateTest(Client& client, const std::string& bodyLines)
{
  Response& response = client.getResponse();
  Headers& headers = response.getHeaders();
  headers.addHeader("Transfer-Encoding", "chunked");
  response.setBody(ft::make_shared<MemoryBuffer>(bodyLines));
  client.getStateHandler().setState<WriteBody>();
}

/* ************************************************************************** */
// GENERATOR HELPERS

std::string generateChunk(std::size_t size)
{
  std::string result;
  result.reserve(size);
  const std::string pattern = "0123456789ABCDEF";
  for (std::size_t i = 0; i < size; ++i) {
    result.push_back(pattern[i % pattern.size()]);
  }
  return result;
}

/* ************************************************************************** */
// CONSUMER HELPERS

std::string consume(Client& client, std::size_t bytes)
{
  client.getStateHandler().getState()->run();
  SmartBuffer& outBuff = client.getOutBuffQueue().getSmartBuffer();
  EXPECT_FALSE(outBuff.usesFile());
  const std::size_t toConsume = std::min(bytes, outBuff.size());
  const std::string data = outBuff.consumeFront(toConsume);
  return data;
}

std::string consumeAll(Client& client, std::size_t consumeSize)
{
  std::string data;
  while (!client.getStateHandler().isDone() ||
         client.getOutBuffQueue().getSmartBuffer().size() > 0) {
    const std::string consumed = consume(client, consumeSize);
    data.append(consumed);
  }
  return data;
}

/* ************************************************************************** */
// EXPECT HELPERS

std::string parseChunkLine(const std::string& data, std::string::size_type& pos)
{
  const std::string::size_type crlfPos = data.find(http::CRLF, pos);
  EXPECT_NE(crlfPos, std::string::npos);
  const std::string line = data.substr(pos, crlfPos - pos);
  pos = crlfPos + std::string(http::CRLF).size();
  return line;
}

void expectChunk(const std::string& data,
                 std::string::size_type& pos,
                 const std::string& expectedSize,
                 const std::string& expectedData)
{
  const std::string chunkSize = parseChunkLine(data, pos);
  EXPECT_EQ(chunkSize, expectedSize);
  const std::string chunkData = parseChunkLine(data, pos);
  EXPECT_EQ(chunkData, expectedData);
}

void expectLastChunk(const std::string& data, std::string::size_type& pos)
{
  expectChunk(data, pos, "0", "");
}

} // namespace

/* ************************************************************************** */
// TESTS

TEST(WriteBodyTester, KeepPollEnabled)
{
  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  const std::string chunk = generateChunk(Client::maxChunk * 2);
  setupStateTest(*client, chunk);
  // One consume runs the state once.
  std::string output;
  {
    SCOPED_TRACE("consume");
    output = consume(*client, chunk.size());
  }

  // Ensure more than maxChunk was written into outBuffer to keep poll enabled.
  EXPECT_GT(output.size(), Client::maxChunk);
}

TEST(WriteBodyTester, MemoryToFileThreshold)
{
  // Ensure amount written into the SmartBuffer keeps it a MemoryBuffer.
  EXPECT_GT(SmartBuffer::getMemoryToFileThreshold(), Client::maxChunk * 2);
}

TEST(WriteBodyTester, BasicTestChunked)
{
  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  const std::string chunk = generateChunk(Client::maxChunk);
  setupStateTest(*client, chunk + chunk);
  std::string output;
  {
    SCOPED_TRACE("consumeAll");
    output = consumeAll(*client, Client::maxChunk);
  }
  const std::string chunkSize = ft::to_string(chunk.size(), std::ios::hex);
  std::string::size_type pos = 0;

  {
    SCOPED_TRACE("chunk1");
    expectChunk(output, pos, chunkSize, chunk);
  }
  {
    SCOPED_TRACE("chunk2");
    expectChunk(output, pos, chunkSize, chunk);
  }
  {
    SCOPED_TRACE("lastChunk");
    expectLastChunk(output, pos);
  }
}

TEST(WriteBodyTester, EmptyBodyChunked)
{
  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  setupStateTest(*client, "");

  std::string output;
  {
    SCOPED_TRACE("consumeAll");
    output = consumeAll(*client, Client::maxChunk);
  }
  std::string::size_type pos = 0;

  {
    SCOPED_TRACE("lastChunk");
    expectLastChunk(output, pos);
  }
}

TEST(WriteBodyTester, SingleByteChunked)
{
  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  setupStateTest(*client, "X");

  std::string output;
  {
    SCOPED_TRACE("consumeAll");
    output = consumeAll(*client, Client::maxChunk);
  }
  std::string::size_type pos = 0;

  {
    SCOPED_TRACE("chunk");
    expectChunk(output, pos, "1", "X");
  }
  {
    SCOPED_TRACE("lastChunk");
    expectLastChunk(output, pos);
  }
}

TEST(WriteBodyTester, LargeAndSmallChunk)
{
  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  const std::string chunk1 = generateChunk(Client::maxChunk);
  const std::string chunk2 = generateChunk(50);
  setupStateTest(*client, chunk1 + chunk2);

  std::string output;
  {
    SCOPED_TRACE("consumeAll");
    output = consumeAll(*client, Client::maxChunk);
  }
  const std::string chunkSize1 = ft::to_string(chunk1.size(), std::ios::hex);
  const std::string chunkSize2 = ft::to_string(chunk2.size(), std::ios::hex);
  std::string::size_type pos = 0;

  {
    SCOPED_TRACE("chunk1");
    expectChunk(output, pos, chunkSize1, chunk1);
  }
  {
    SCOPED_TRACE("chunk2");
    expectChunk(output, pos, chunkSize2, chunk2);
  }
  {
    SCOPED_TRACE("lastChunk");
    expectLastChunk(output, pos);
  }
}

TEST(WriteBodyTester, ThreeChunks)
{
  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  const std::string chunk = generateChunk(Client::maxChunk);
  setupStateTest(*client, chunk + chunk + chunk);

  std::string output;
  {
    SCOPED_TRACE("consumeAll");
    output = consumeAll(*client, Client::maxChunk);
  }
  const std::string chunkSize = ft::to_string(chunk.size(), std::ios::hex);
  std::string::size_type pos = 0;

  {
    SCOPED_TRACE("chunk1");
    expectChunk(output, pos, chunkSize, chunk);
  }
  {
    SCOPED_TRACE("chunk2");
    expectChunk(output, pos, chunkSize, chunk);
  }
  {
    SCOPED_TRACE("chunk3");
    expectChunk(output, pos, chunkSize, chunk);
  }
  {
    SCOPED_TRACE("lastChunk");
    expectLastChunk(output, pos);
  }
}

TEST(WriteBodyTester, SlowConsumerChunked)
{
  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  const std::string chunk = generateChunk(Client::maxChunk);
  setupStateTest(*client, chunk + chunk);

  // Consume in small increments
  const std::size_t consumeSize = 100;
  std::string output;
  {
    SCOPED_TRACE("consumeAll");
    output = consumeAll(*client, consumeSize);
  }
  const std::string chunkSize = ft::to_string(chunk.size(), std::ios::hex);
  std::string::size_type pos = 0;

  {
    SCOPED_TRACE("chunk1");
    expectChunk(output, pos, chunkSize, chunk);
  }
  {
    SCOPED_TRACE("chunk2");
    expectChunk(output, pos, chunkSize, chunk);
  }
  {
    SCOPED_TRACE("lastChunk");
    expectLastChunk(output, pos);
  }
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
