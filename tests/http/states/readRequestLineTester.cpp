#include "libftpp/memory.hpp"
#include "libftpp/utility.hpp"
#include <client/Client.hpp>
#include <gtest/gtest.h>
#include <http/Request.hpp>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <string>
#include <utils/state/IState.hpp>

// NOLINTBEGIN

namespace {
ft::unique_ptr<Client> StateTest(const std::string& requestLine)
{
  ft::unique_ptr<Client> client = ft::make_unique<Client>();
  client->getInBuff().add(requestLine);
  client->getStateHandler().setState<ReadRequestLine>();
  client->getStateHandler().getState()->run();
  return ft::move(client);
}
}

TEST(ReadRequestLineTester, BasicRequests)
{
  std::string line("GET http://test/ HTTP/1.0\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Request& request = client->getRequest();
  EXPECT_EQ(request.getMethod(), Request::GET);
  EXPECT_EQ(request.getUri().getScheme(), "http");
  EXPECT_EQ(request.getUri().getAuthority(), "test");
  EXPECT_EQ(request.getUri().getPath(), "/");
  EXPECT_EQ(request.getVersion(), "HTTP/1.0");
}

TEST(ReadRequestLineTester, OriginForm)
{
  std::string line("GET /where?test#frag HTTP/1.1\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Request& request = client->getRequest();
  EXPECT_EQ(request.getMethod(), Request::GET);
  EXPECT_EQ(request.getUri().getScheme(), "");
  EXPECT_EQ(request.getUri().getAuthority(), "");
  EXPECT_EQ(request.getUri().getPath(), "/where");
  EXPECT_EQ(request.getUri().getQuery(), "?test");
  EXPECT_EQ(request.getUri().getFragment(), "#frag");
  EXPECT_EQ(request.getVersion(), "HTTP/1.1");
}

TEST(ReadRequestLineTester, AbsoluteForm)
{
  std::string line("GET "
                   "http://www.example.org/pub/WWW/TheProject.html?test#frag "
                   "HTTP/1.1\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Request& request = client->getRequest();
  EXPECT_EQ(request.getMethod(), Request::GET);
  EXPECT_EQ(request.getUri().getScheme(), "http");
  EXPECT_EQ(request.getUri().getAuthority(), "www.example.org");
  EXPECT_EQ(request.getUri().getPath(), "/pub/WWW/TheProject.html");
  EXPECT_EQ(request.getUri().getQuery(), "?test");
  EXPECT_EQ(request.getUri().getFragment(), "#frag");
  EXPECT_EQ(request.getVersion(), "HTTP/1.1");
}

// NOLINTEND

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
