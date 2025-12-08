#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Request.hpp>
#include <http/Response.hpp>
#include <http/StatusCode.hpp>
#include <http/http.hpp>
#include <http/states/readHeaderLines/ReadHeaderLines.hpp>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <http/utils/HeaderParser.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <testUtils.hpp>
#include <utils/buffer/MemoryBuffer.hpp>
#include <utils/state/IState.hpp>

#include <gtest/gtest.h>
#include <string>

using testUtils::makeString;

// NOLINTBEGIN

namespace {

ft::unique_ptr<Client> StateTest(const std::string& requestLine)
{
  ft::unique_ptr<Client> client = ft::make_unique<Client>();
  client->getInBuff().append(requestLine);
  client->getStateHandler().setState<ReadHeaderLines>();
  client->getStateHandler().getState()->run();
  return ft::move(client);
}

HeaderParser::Result validateOnly(const std::string& line)
{
  MemoryBuffer buffer(line);
  HeaderParser _headerParser;
  return _headerParser.validateHeaderPart(buffer);
}

} // namespace

TEST(ReadHeaderLinesTester, BasicHeaders)
{
  std::string line("Host: webserv\r\n"
                   "Content-Length: 7\r\n"
                   "\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Request& request = client->getRequest();
  Headers& headers = request.getHeaders();

  std::string value;
  EXPECT_NO_THROW(value = headers.at("Host"));
  EXPECT_EQ(value, "webserv");
  EXPECT_NO_THROW(value = headers.at("Content-Length"));
  EXPECT_EQ(value, "7");
}

TEST(ReadHeaderLinesTester, HeaderList)
{
  std::string headerValue = "tes, test";
  // std::string headerValue = "\"Chromium\";v=\"140\", "
  //                           "\"Not=A?Brand\";v=\"24\", "
  //                           "\"Google Chrome\";v=\"140\"";

  std::string line("Host: webserv\r\n");
  line.append("sec-ch-ua: ");
  line.append(headerValue);
  line.append(http::CRLF);
  line.append(http::CRLF);
  ft::unique_ptr<Client> client = StateTest(line);
  Request& request = client->getRequest();
  Headers& headers = request.getHeaders();

  std::string value;
  EXPECT_NO_THROW(value = headers.at("Host"));
  EXPECT_EQ(value, "webserv");
  EXPECT_NO_THROW(value = headers.at("sec-ch-ua"));
  EXPECT_EQ(value, headerValue);
}

TEST(ReadHeaderLinesTester, HeaderValueTooLarge)
{
  // check is ONE_HEADER > MAX_FIELDLINE_SIZE

  ReadHeaderLines::setMaxFieldLineSize(37);

  std::string line("Host: webserv\r\n"
                   "Header_test: loooooooooooooooooooong\r\n" // size 38
                   "\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response = client->getResponse();
  const StatusCode& statusCode = response.getStatusCode();

  EXPECT_EQ(statusCode, StatusCode::RequestHeaderFieldsTooLarge);

  ReadHeaderLines::resetMaxFieldLineSize();
}

TEST(ReadHeaderLinesTester, HeadersTooLarge)
{
  // check is ALL_HEADERS > MAX_HEADER_SIZE

  ReadHeaderLines::setMaxHeaderSize(91);

  std::string line("Header1: loooooooooooooooong\r\n" // size 30
                   "Header2: loooooooooooooooong\r\n" // size 30
                   "Header3: loooooooooooooooong\r\n" // size 30
                   "\r\n");                           // size 2
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response = client->getResponse();
  const StatusCode& statusCode = response.getStatusCode();

  EXPECT_EQ(statusCode, StatusCode::RequestHeaderFieldsTooLarge);

  ReadHeaderLines::resetMaxHeaderSize();
}

TEST(ReadHeaderLinesTester, MultipleHostHeaders)
{
  std::string line("TestHeader1: hi\r\n"
                   "Host: webserv\r\n"
                   "Host: somethingElse\r\n"
                   "TestHeader: 7\r\n"
                   "\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response = client->getResponse();
  const StatusCode& statusCode = response.getStatusCode();

  EXPECT_EQ(statusCode, StatusCode::BadRequest);
}

TEST(ReadHeaderLinesTester, TransferEncAndContentLen)
{
  std::string line("TestHeader1: hi\r\n"
                   "Transfer-Encoding: chunked\r\n"
                   "Content-Length: 3\r\n"
                   "TestHeader: 7\r\n"
                   "\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response = client->getResponse();
  const StatusCode& statusCode = response.getStatusCode();

  EXPECT_EQ(statusCode, StatusCode::BadRequest);
}

TEST(ReadHeaderLinesTester, InvalidContentLength)
{
  std::string line("TestHeader1: hi\r\n"
                   "Content-Length: 7a\r\n"
                   "\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response = client->getResponse();
  const StatusCode& statusCode = response.getStatusCode();
  EXPECT_EQ(statusCode, StatusCode::BadRequest);
}

TEST(ReadHeaderLinesTester, InvalidTransferEncoding)
{
  std::string line("TestHeader1: hi\r\n"
                   "Transfer-Encoding: chunked;\r\n"
                   "\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response = client->getResponse();
  const StatusCode& statusCode = response.getStatusCode();
  EXPECT_EQ(statusCode, StatusCode::BadRequest);
}

TEST(ReadHeaderLinesTester, NulByteInHeaderName)
{
  std::string line = makeString("Host: webserv\r\n"
                                "Header\0Name: value\r\n"
                                "\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response = client->getResponse();
  const StatusCode& statusCode = response.getStatusCode();

  EXPECT_EQ(statusCode, StatusCode::BadRequest);
}

TEST(ReadHeaderLinesTester, NulByteInHeaderValue)
{
  std::string line = makeString("Host: webserv\r\n"
                                "Content-Type: text\0/html\r\n"
                                "\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response = client->getResponse();
  const StatusCode& statusCode = response.getStatusCode();

  EXPECT_EQ(statusCode, StatusCode::BadRequest);
}

TEST(ReadHeaderLinesTester, NulByteAtStartOfHeader)
{
  std::string line = makeString("\0Host: webserv\r\n"
                                "\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response = client->getResponse();
  const StatusCode& statusCode = response.getStatusCode();

  EXPECT_EQ(statusCode, StatusCode::BadRequest);
}

TEST(ReadHeaderLinesTester, NulByteBeforeColon)
{
  std::string line = makeString("Host: webserv\r\n"
                                "Content-Length\0: 7\r\n"
                                "\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response = client->getResponse();
  const StatusCode& statusCode = response.getStatusCode();

  EXPECT_EQ(statusCode, StatusCode::BadRequest);
}

TEST(ReadHeaderLinesTester, NulByteInHostHeader)
{
  std::string line = makeString("Host: web\0serv\r\n"
                                "\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response = client->getResponse();
  const StatusCode& statusCode = response.getStatusCode();

  EXPECT_EQ(statusCode, StatusCode::BadRequest);
}

TEST(ReadHeaderLinesTester, NulByteInContentLengthHeader)
{
  std::string line = makeString("Host: webserv\r\n"
                                "Content-Length: \0007\r\n"
                                "\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response = client->getResponse();
  const StatusCode& statusCode = response.getStatusCode();

  EXPECT_EQ(statusCode, StatusCode::BadRequest);
}

TEST(ReadHeaderLinesTester, ValidateHeaderPartValid)
{
  std::string line = "Host: webserv\r\n"
                     "Content-Length: value\r\n"
                     "\r\n";
  EXPECT_EQ(validateOnly(line), HeaderParser::EndOfHeaders);

  line = "\r\n";
  EXPECT_EQ(validateOnly(line), HeaderParser::EndOfHeaders);
}

TEST(ReadHeaderLinesTester, ValidateHeaderPartEndOfBuffer)
{
  std::string line;
  line = "Host";
  EXPECT_EQ(validateOnly(line), HeaderParser::EndOfBuffer);

  line = "Host: webserv\r\n";
  EXPECT_EQ(validateOnly(line), HeaderParser::EndOfBuffer);

  line = "Host: webserv\r\n"
         "Content-Length: value\r\n";
  EXPECT_EQ(validateOnly(line), HeaderParser::EndOfBuffer);

  line = "Host: webserv\r\n"
         "Content-Length: value\r\n"
         "\r";
  EXPECT_EQ(validateOnly(line), HeaderParser::EndOfBuffer);
}

TEST(ReadHeaderLinesTester, ValidateHeaderPartSyntaxError)
{
  std::string line = "Host: webserv\r\n"
                     "Content-Length : value\r\n"
                     "\r\n";
  EXPECT_EQ(validateOnly(line), HeaderParser::SyntaxError);
}

// NOLINTEND

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
