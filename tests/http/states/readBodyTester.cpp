#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Request.hpp>
#include <http/Response.hpp>
#include <http/StatusCode.hpp>
#include <http/states/readBody/ReadBody.hpp>
#include <http/states/readHeaderLines/ReadHeaderLines.hpp>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/string.hpp>
#include <testUtils.hpp>
#include <utils/buffer/SmartBuffer.hpp>
#include <utils/state/IState.hpp>

#include <gtest/gtest.h>
#include <string>

using testUtils::makeString;

namespace {

void StateTest(Client& client, const std::string& bodyLines)
{
  client.getInBuff().append(bodyLines);
  client.getStateHandler().setState<ReadBody>();
  client.getStateHandler().getState()->run();
}

} // namespace

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
  const std::string body = buff.consumeFront(buff.size());
  EXPECT_EQ(body, data);
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
  const std::string body = buff.consumeFront(buff.size());
  EXPECT_EQ(body, data1);
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
  const std::string body = buff.consumeFront(buff.size());
  EXPECT_EQ(body, data);

  const Headers& trailers = request.getTrailers();
  std::string value;
  EXPECT_NO_THROW(value = trailers.at("Trailer1"));
  EXPECT_EQ(value, "value1");
  EXPECT_NO_THROW(value = trailers.at("Trailer2"));
  EXPECT_EQ(value, "value2");
}

TEST(ReadBodyTester, TransferEncodingEmpty)
{
  {
    const ft::unique_ptr<Client> client = ft::make_unique<Client>();
    client->getRequest().getHeaders().addHeader("Transfer-Encoding", "");
    StateTest(*client, "not important");
    const Response& response = client->getResponse();
    EXPECT_EQ(response.getStatusCode(), StatusCode::BadRequest);
  }
}

TEST(ReadBodyTester, ChunkedIsNotFinalEncoding)
{
  {
    const ft::unique_ptr<Client> client = ft::make_unique<Client>();
    client->getRequest().getHeaders().addHeader("Transfer-Encoding", "chunke");
    StateTest(*client, "not important");
    const Response& response = client->getResponse();
    EXPECT_EQ(response.getStatusCode(), StatusCode::BadRequest);
  }
  {
    const ft::unique_ptr<Client> client = ft::make_unique<Client>();
    client->getRequest().getHeaders().addHeader("Transfer-Encoding",
                                                "chunked, zip");
    StateTest(*client, "not important");
    const Response& response = client->getResponse();
    EXPECT_EQ(response.getStatusCode(), StatusCode::BadRequest);
  }
}

TEST(ReadBodyTester, TransferEncodingMultipleChunked)
{
  {
    const ft::unique_ptr<Client> client = ft::make_unique<Client>();
    client->getRequest().getHeaders().addHeader("Transfer-Encoding",
                                                "chunked, CHUNKED");
    StateTest(*client, "not important");
    const Response& response = client->getResponse();
    EXPECT_EQ(response.getStatusCode(), StatusCode::BadRequest);
  }
}

TEST(ReadBodyTester, TransferEncodingNotImplemented)
{
  {
    const ft::unique_ptr<Client> client = ft::make_unique<Client>();
    client->getRequest().getHeaders().addHeader("Transfer-Encoding",
                                                ", zip, chunked");
    StateTest(*client, "not important");
    const Response& response = client->getResponse();
    EXPECT_EQ(response.getStatusCode(), StatusCode::NotImplemented);
  }
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
  const std::string body = buff.consumeFront(buff.size());
  EXPECT_EQ(body, data);
}

TEST(ReadBodyTester, FixedLengthSpecialBytes)
{
  const std::string data = makeString("foo\0bar"         // NUL byte
                                      "\t\n\r\v\f\r\n"   // Whitespace
                                      "\x01\x02\x1F\x7F" // Control chars
                                      "\x80\x90\xA0\xB0" // Non-ASCII bytes
                                      "\xC0\xD0\xE0\xF0" // More non-ASCII bytes
                                      "\xFF\xFE\xFD\xFC" // High non-ASCII bytes
  );

  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  client->getRequest().getHeaders().addHeader("Content-Length",
                                              ft::to_string(data.size()));
  StateTest(*client, data);
  Request& request = client->getRequest();
  SmartBuffer& buff = request.getBody();
  const std::string body = buff.consumeFront(buff.size());
  EXPECT_EQ(body, data);
}

TEST(ReadBodyTester, ChunkedSpecialBytes)
{
  const std::string chunk1 = makeString("foo\0bar");
  const std::string chunk2 = makeString("\x01\x7F\x80\xC2\xFF");

  std::string line;
  line.append("7\r\n");
  line.append(chunk1);
  line.append("\r\n");
  line.append("5\r\n");
  line.append(chunk2);
  line.append("\r\n");
  line.append("0\r\n");
  line.append("\r\n");

  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  client->getRequest().getHeaders().addHeader("Transfer-Encoding", "chunked");
  StateTest(*client, line);
  Request& request = client->getRequest();
  SmartBuffer& buff = request.getBody();
  const std::string expectedData = chunk1 + chunk2;
  ASSERT_TRUE(expectedData.size() == (chunk1.size() + chunk2.size()));
  const std::string body = buff.consumeFront(buff.size());
  EXPECT_EQ(body, expectedData);
}

TEST(ReadBodyTester, FixedLengthInvalid)
{
  const std::string line("0123456789\r\n");

  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  client->getRequest().getHeaders().addHeader("Content-Length",
                                              "999999999999999999999999999999");
  StateTest(*client, line);
  const Response& response = client->getResponse();

  EXPECT_EQ(response.getStatusCode(), StatusCode::ContentTooLarge);
}

// TODO add when content-length grama available
TEST(ReadBodyTester, DISABLED_FixedLengthInvalid)
{
  const std::string line("0123456789\r\n");

  const ft::unique_ptr<Client> client = ft::make_unique<Client>();
  client->getRequest().getHeaders().addHeader("Content-Length", "10 afdadf");
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
