#include "http/Response.hpp"
#include "http/StatusCode.hpp"
#include "http/states/readBody/ReadBody.hpp"
#include "utils/buffer/IBuffer.hpp"
#include "utils/buffer/SmartBuffer.hpp"
#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Request.hpp>
#include <http/states/readHeaderLines/ReadHeaderLines.hpp>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <libftpp/memory.hpp>
#include <utils/state/IState.hpp>

#include <gtest/gtest.h>
#include <string>

namespace {
void StateTest(Client& client, const std::string& bodyLines)
{
  client.getInBuff().append(bodyLines);
  client.getStateHandler().setState<ReadBody>();
  client.getStateHandler().getState()->run();
}
}

TEST(ReadBodyTester, Chunked)
{
  const std::string data1 = "0123456789\n";
  const std::string data2 = "0123456789";

  std::string line;
  line.append("b\r\n");
  line.append(data1);
  line.append("\r\n");
  line.append("a\r\n");
  line.append(data2);
  line.append("\r\n");
  line.append("0\r\n");
  line.append("\r\n");

  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  client->getRequest().getHeaders().addHeader("Transfer-Encoding", "chunked");
  StateTest(*client, line);
  Request& request = client->getRequest();
  SmartBuffer& buff = request.getBody();
  const std::string data = data1 + data2;
  IBuffer::ExpectStr body = buff.consumeFront(buff.size());
  EXPECT_TRUE(body.has_value());
  if (body.has_value()) {
    EXPECT_EQ(*body, data);
  }
}

TEST(ReadBodyTester, ChunkedSingleChunk)
{
  const std::string data1 = "helllo woooooooooooooorrld";

  std::string line;
  line.append("1a\r\n");
  line.append(data1);
  line.append("\r\n");
  line.append("0\r\n");
  line.append("\r\n");

  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  client->getRequest().getHeaders().addHeader("Transfer-Encoding", "chunked");
  StateTest(*client, line);
  Request& request = client->getRequest();
  SmartBuffer& buff = request.getBody();
  IBuffer::ExpectStr body = buff.consumeFront(buff.size());
  EXPECT_TRUE(body.has_value());
  if (body.has_value()) {
    EXPECT_EQ(*body, data1);
  }
}

TEST(ReadBodyTester, ChunkedWithTrailer)
{
  const std::string data1 = "0123456789\n";
  const std::string data2 = "0123456789";

  std::string line;
  line.append("B\r\n");
  line.append(data1);
  line.append("\r\n");
  line.append("a\r\n");
  line.append(data2);
  line.append("\r\n");
  line.append("0\r\n");
  line.append("Trailer1: value1\r\n");
  line.append("Trailer2: value2\r\n");
  line.append("\r\n");

  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  client->getRequest().getHeaders().addHeader("Transfer-Encoding", "chunked");
  StateTest(*client, line);
  Request& request = client->getRequest();
  SmartBuffer& buff = request.getBody();
  const std::string data = data1 + data2;
  IBuffer::ExpectStr body = buff.consumeFront(buff.size());
  EXPECT_TRUE(body.has_value());
  if (body.has_value()) {
    EXPECT_EQ(*body, data);
  }

  const Headers& headers = request.getHeaders();
  std::string value;
  EXPECT_NO_THROW(value = headers.at("Trailer1"));
  EXPECT_EQ(value, "value1");
  EXPECT_NO_THROW(value = headers.at("Trailer2"));
  EXPECT_EQ(value, "value2");
}

TEST(ReadBodyTester, FixedLength)
{
  const std::string line("0123456789\r\n");

  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  client->getRequest().getHeaders().addHeader("Content-Length", "10");
  StateTest(*client, line);
  Request& request = client->getRequest();
  SmartBuffer& buff = request.getBody();
  const std::string data = "0123456789";
  IBuffer::ExpectStr body = buff.consumeFront(buff.size());
  EXPECT_TRUE(body.has_value());
  if (body.has_value()) {
    EXPECT_EQ(*body, data);
  }
}

TEST(ReadBodyTester, FixedLengthInvalid)
{
  const std::string line("0123456789\r\n");

  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  client->getRequest().getHeaders().addHeader("Content-Length", "10 99");
  StateTest(*client, line);
  const Response& response = client->getResponse();

  EXPECT_EQ(response.getStatusCode(), StatusCode::BadRequest);
}

TEST(ReadBodyTester, DISABLED_FixedLengthMaxBodyExceeded)
{
  // todo set max body size to make this test work

  const std::string line("01234567890123456789"
                         "01234567890123456789"
                         "01234567890123456789"
                         "01234567890123456789"
                         "01234567890123456789"
                         "01234567890123456789"
                         "01234567890123456789");

  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  client->getRequest().getHeaders().addHeader("Content-Length", "308");
  StateTest(*client, line);
  const Response& response = client->getResponse();

  EXPECT_EQ(response.getStatusCode(), StatusCode::ContentTooLarge);
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
