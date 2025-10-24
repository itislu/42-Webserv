#include "http/Request.hpp"
#include "http/states/readHeaderLines/ReadHeaderLines.hpp"
#include <client/Client.hpp>
#include <gtest/gtest.h>
#include <http/states/readStartLine/ReadStartLine.hpp>
#include <string>
#include <utils/state/IState.hpp>

// NOLINTBEGIN

namespace {
Client* StateTest(std::string& startLine)
{
  Client* client = new Client();
  client->getInBuff().add(startLine);
  IState<Client>* state = new ReadHeaderLines(client);
  state->run();
  delete state;
  return client;
}
}

TEST(ReadStartLineTester, BasicRequests)
{
  std::string line("Host: webserv\r\n"
                   "Conten-Length: 7\r\n"
                   "\r\n");
  Client* client = StateTest(line);
  Request& request = client->getRequest();
  Request::HeaderMap& headers = request.getHeaders();

  EXPECT_EQ(headers["Host"], "webserv");
  EXPECT_EQ(headers["Conten-Length"], "7");
  delete client;
}

// NOLINTEND

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
