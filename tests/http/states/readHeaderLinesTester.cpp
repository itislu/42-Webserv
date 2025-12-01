#include "http/Response.hpp"
#include "http/StatusCode.hpp"
#include "http/http.hpp"
#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Request.hpp>
#include <http/states/readHeaderLines/ReadHeaderLines.hpp>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <testUtils.hpp>
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
}

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

TEST(ReadHeaderLinesTester, DISABLED_HeaderValueTooLarge)
{
  // todo set max HeaderFieldLineLength

  std::string line("Host: webserv\r\n"
                   "Header_test: loooooooooooooooooooong\r\n"
                   "\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response = client->getResponse();
  const StatusCode& statusCode = response.getStatusCode();

  EXPECT_EQ(statusCode, StatusCode::RequestHeaderFieldsTooLarge);
}

TEST(ReadHeaderLinesTester, DISABLED_HeadersTooLarge)
{
  // todo set max HeaderLength

  std::string line("Host: webserv\r\n"
                   "Header1: loooooooooooooooooooong\r\n"
                   "Header2: loooooooooooooooooooong\r\n"
                   "Header3: loooooooooooooooooooong\r\n"
                   "\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response = client->getResponse();
  const StatusCode& statusCode = response.getStatusCode();

  EXPECT_EQ(statusCode, StatusCode::RequestHeaderFieldsTooLarge);
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

// NOLINTEND

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
