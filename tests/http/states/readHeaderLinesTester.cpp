#include "http/http.hpp"
#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Request.hpp>
#include <http/Response.hpp>
#include <http/StatusCode.hpp>
#include <http/states/readHeaderLines/ReadHeaderLines.hpp>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/state/IState.hpp>

#include <gtest/gtest.h>
#include <string>

// NOLINTBEGIN

namespace {
ft::unique_ptr<Client> StateTest(const std::string& requestLine)
{
  ft::unique_ptr<Client> client = ft::make_unique<Client>();
  client->getInBuff().add(requestLine);
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

  EXPECT_EQ(headers.at("Host"), "webserv");
  EXPECT_EQ(headers.at("Content-Length"), "7");
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

  EXPECT_NO_THROW(EXPECT_EQ(headers.at("Host"), "webserv"));
  EXPECT_NO_THROW(EXPECT_EQ(headers.at("sec-ch-ua"), headerValue));
}

/**
 * obs-fold     = OWS CRLF RWS
 *
 * > A server that receives an obs-fold in a request message that is not within
 * > a "message/http" container MUST either reject the message by sending a 400
 * > (Bad Request), preferably with a representation explaining that obsolete
 * > line folding is unacceptable, or replace each received obs-fold with one or
 * > more SP octets prior to interpreting the field value or forwarding the
 * > message downstream.
 * https://datatracker.ietf.org/doc/html/rfc9112#name-obsolete-line-folding
 *
 * Line folding only MUST be supported within a "container" (i.e., in bodies of
 * "message/http" requests).
 * https://datatracker.ietf.org/doc/html/rfc9112#name-media-type-message-http
 *
 * To simplify parsing, and because line folding is deprecated since RFC 7230,
 * we choose to reject requests with obs-folding instead of unfolding them.
 */
TEST(ReadHeaderLinesTester, ObsoleteLineFolding)
{
  std::string line("Host: webserv\r\n"
                   "Content-Length: 0\r\n"
                   "Content-Type: message/http\r\n"
                   " Foo: bar\r\n"
                   "\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response = client->getResponse();

  EXPECT_EQ(response.getStatusCode(), StatusCode::BadRequest);
}

// NOLINTEND

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
