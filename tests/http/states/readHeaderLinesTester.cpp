#include "http/Request.hpp"
#include "http/states/readHeaderLines/ReadHeaderLines.hpp"
#include "libftpp/memory.hpp"
#include "libftpp/utility.hpp"
#include <client/Client.hpp>
#include <gtest/gtest.h>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <string>
#include <utils/state/IState.hpp>

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

TEST(ReadRequestLineTester, BasicHeaders)
{
  std::string line("Host: webserv\r\n"
                   "Content-Length: 7\r\n"
                   "\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Request& request = client->getRequest();
  Request::HeaderMap& headers = request.getHeaders();

  EXPECT_EQ(headers["Host"], "webserv");
  EXPECT_EQ(headers["Content-Length"], "7");
}

// NOLINTEND

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
