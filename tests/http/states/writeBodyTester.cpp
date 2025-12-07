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

/* ************************************************************************** */
// CONSUMER HELPERS

#define CONSUME(CLIENT, BYTES)                                                 \
  [&_client = (CLIENT), _bytes = (BYTES)]() -> std::string {                   \
    _client.getStateHandler().getState()->run();                               \
    SmartBuffer& outBuff = _client.getOutBuffQueue().getSmartBuffer();         \
    EXPECT_FALSE(outBuff.usesFile());                                          \
    const std::size_t toConsume = std::min(_bytes, outBuff.size());            \
    const std::string data = outBuff.consumeFront(toConsume);                  \
    return data;                                                               \
  }()

#define CONSUME_ALL(CLIENT, CONSUME_SIZE)                                      \
  [&_client = (CLIENT), _consumeSize = (CONSUME_SIZE)]() -> std::string {      \
    std::string data;                                                          \
    while (!_client.getStateHandler().isDone() ||                              \
           _client.getOutBuffQueue().getSmartBuffer().size() > 0) {            \
      const std::string consumed = CONSUME(_client, _consumeSize);             \
      data.append(consumed);                                                   \
    }                                                                          \
    return data;                                                               \
  }()

/* ************************************************************************** */
// EXPECT HELPERS

#define PARSE_CHUNK_LINE(DATA, POS)                                            \
  [&_data = (DATA), &_pos = (POS)]() -> std::string {                          \
    const std::string::size_type crlfPos = _data.find(http::CRLF, _pos);       \
    EXPECT_NE(crlfPos, std::string::npos);                                     \
    const std::string line = _data.substr(_pos, crlfPos - _pos);               \
    _pos = crlfPos + std::string(http::CRLF).size();                           \
    return line;                                                               \
  }()

#define EXPECT_CHUNK(DATA, POS, EXPECTED_SIZE, EXPECTED_DATA)                  \
  [&_data = (DATA),                                                            \
   &_pos = (POS),                                                              \
   &_expectedSize = (EXPECTED_SIZE),                                           \
   &_expectedData = (EXPECTED_DATA)]() -> void {                               \
    const std::string chunkSize = PARSE_CHUNK_LINE(_data, _pos);               \
    EXPECT_EQ(chunkSize, _expectedSize);                                       \
    const std::string chunkData = PARSE_CHUNK_LINE(_data, _pos);               \
    EXPECT_EQ(chunkData, _expectedData);                                       \
  }()

#define EXPECT_LAST_CHUNK(DATA, POS) EXPECT_CHUNK((DATA), (POS), "0", "")

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

}

/* ************************************************************************** */
// TESTS

TEST(WriteBodyTester, KeepPollEnabled)
{
  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  const std::string chunk = generateChunk(Client::maxChunk * 2);
  setupStateTest(*client, chunk);
  // One consume runs the state once.
  const std::string output = CONSUME(*client, chunk.size());

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
  const std::string output = CONSUME_ALL(*client, Client::maxChunk);

  const std::string chunkSize = ft::to_string(chunk.size(), std::ios::hex);
  std::string::size_type pos = 0; // NOLINT(misc-const-correctness)

  EXPECT_CHUNK(output, pos, chunkSize, chunk);
  EXPECT_CHUNK(output, pos, chunkSize, chunk);
  EXPECT_LAST_CHUNK(output, pos);
}

TEST(WriteBodyTester, EmptyBodyChunked)
{
  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  setupStateTest(*client, "");

  const std::string output = CONSUME_ALL(*client, Client::maxChunk);
  std::string::size_type pos = 0; // NOLINT(misc-const-correctness)

  EXPECT_LAST_CHUNK(output, pos);
}

TEST(WriteBodyTester, SingleByteChunked)
{
  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  setupStateTest(*client, "X");

  const std::string output = CONSUME_ALL(*client, Client::maxChunk);
  std::string::size_type pos = 0; // NOLINT(misc-const-correctness)

  EXPECT_CHUNK(output, pos, "1", "X");
  EXPECT_LAST_CHUNK(output, pos);
}

TEST(WriteBodyTester, LargeAndSmallChunk)
{
  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  const std::string chunk1 = generateChunk(Client::maxChunk);
  const std::string chunk2 = generateChunk(50);
  setupStateTest(*client, chunk1 + chunk2);

  const std::string output = CONSUME_ALL(*client, Client::maxChunk);
  const std::string chunkSize1 = ft::to_string(chunk1.size(), std::ios::hex);
  const std::string chunkSize2 = ft::to_string(chunk2.size(), std::ios::hex);
  std::string::size_type pos = 0; // NOLINT(misc-const-correctness)

  EXPECT_CHUNK(output, pos, chunkSize1, chunk1);
  EXPECT_CHUNK(output, pos, chunkSize2, chunk2);
  EXPECT_LAST_CHUNK(output, pos);
}

TEST(WriteBodyTester, ThreeChunks)
{
  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  const std::string chunk = generateChunk(Client::maxChunk);
  setupStateTest(*client, chunk + chunk + chunk);

  const std::string output = CONSUME_ALL(*client, Client::maxChunk);
  const std::string chunkSize = ft::to_string(chunk.size(), std::ios::hex);
  std::string::size_type pos = 0; // NOLINT(misc-const-correctness)

  EXPECT_CHUNK(output, pos, chunkSize, chunk);
  EXPECT_CHUNK(output, pos, chunkSize, chunk);
  EXPECT_CHUNK(output, pos, chunkSize, chunk);
  EXPECT_LAST_CHUNK(output, pos);
}

TEST(WriteBodyTester, SlowConsumerChunked)
{
  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  const std::string chunk = generateChunk(Client::maxChunk);
  setupStateTest(*client, chunk + chunk);

  // Consume in small increments
  const std::size_t consumeSize = 100;
  const std::string output = CONSUME_ALL(*client, consumeSize);
  const std::string chunkSize = ft::to_string(chunk.size(), std::ios::hex);
  std::string::size_type pos = 0; // NOLINT(misc-const-correctness)

  EXPECT_CHUNK(output, pos, chunkSize, chunk);
  EXPECT_CHUNK(output, pos, chunkSize, chunk);
  EXPECT_LAST_CHUNK(output, pos);
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
