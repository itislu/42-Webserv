#include "http/http.hpp"
#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Request.hpp>
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

// NOLINTEND

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
