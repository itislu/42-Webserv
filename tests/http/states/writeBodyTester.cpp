#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Response.hpp>
#include <http/http.hpp>
#include <http/states/readHeaderLines/ReadHeaderLines.hpp>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <http/states/writeBody/WriteBody.hpp>
#include <libftpp/memory.hpp>
#include <utils/buffer/MemoryBuffer.hpp>
#include <utils/buffer/SmartBuffer.hpp>
#include <utils/state/IState.hpp>

#include <gtest/gtest.h>
#include <string>

const std::string chunk = "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF01"
                          "23456789ABCDEF0123456789AB"
                          "CDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCD"
                          "EF0123456789ABCDEF01234567"
                          "89ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789"
                          "ABCDEF0123456789ABCDEF0123"
                          "456789ABCDEF0123456789ABCDEF0123456789ABCDEF012345"
                          "6789ABCDEF0123456789ABCDEF"
                          "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF01"
                          "23456789ABCDEF0123456789AB"
                          "CDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCD"
                          "EF0123456789ABCDEF01234567"
                          "89ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789"
                          "ABCDEF0123456789ABCDEF0123"
                          "456789ABCDEF0123456789ABCDEF0123456789ABCDEF012345"
                          "6789ABCDEF0123456789ABCDEF"
                          "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF01"
                          "23456789ABCDEF0123456789AB"
                          "CDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCD"
                          "EF0123456789ABCDEF01234567"
                          "89ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789"
                          "ABCDEF0123456789ABCDEF0123"
                          "456789ABCDEF0123456789ABCDEF0123456789ABCDEF012345"
                          "6789ABCDEF0123456789ABCDEF"
                          "0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF01"
                          "23456789ABCDEF0123456789AB"
                          "CDEF0123456789ABCDEF0123456789ABCDEF";

namespace {
void StateTest(Client& client, const std::string& bodyLines)
{
  Response& response = client.getResponse();
  Headers& headers = response.getHeaders();
  headers.addHeader("Transfer-Encoding", "chunked");
  response.setBody(ft::make_shared<MemoryBuffer>(bodyLines));
  client.getStateHandler().setState<WriteBody>();
  client.getStateHandler().getState()->run();
}
}

TEST(WriteBodyTester, BasicTest)
{
  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  StateTest(*client, chunk + chunk);

  SmartBuffer& buffer = client->getOutBuffQueue().getSmartBuffer();

  // chunk 1
  EXPECT_EQ(buffer.consumeFront(5), "400\r\n");
  EXPECT_EQ(buffer.consumeFront(chunk.size()), chunk);
  EXPECT_EQ(buffer.consumeFront(2), http::CRLF);

  // chunk 2
  EXPECT_EQ(buffer.consumeFront(5), "400\r\n");
  EXPECT_EQ(buffer.consumeFront(chunk.size()), chunk);
  EXPECT_EQ(buffer.consumeFront(2), http::CRLF);

  // end chunk
  EXPECT_EQ(buffer.consumeFront(3), "0\r\n");

  // end of line
  EXPECT_EQ(buffer.consumeFront(2), http::CRLF);
}

TEST(WriteBodyTester, EmptyBody)
{
  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  StateTest(*client, "");

  SmartBuffer& buffer = client->getOutBuffQueue().getSmartBuffer();

  // end chunk
  EXPECT_EQ(buffer.consumeFront(3), "0\r\n");

  // end of line
  EXPECT_EQ(buffer.consumeFront(2), http::CRLF);
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
