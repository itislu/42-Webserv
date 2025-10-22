
#include <utils/state/IState.hpp>
#include <client/Client.hpp>
#include <gtest/gtest.h>
#include <http/Request.hpp>
#include <http/states/readStartLine/ReadStartLine.hpp>
#include <string>

// NOLINTBEGIN

namespace {
Client StateTest(std::string& startLine)
{
  Client client = Client();
  client.getInBuff().add(startLine);
  IState<Client>* state = new ReadStartLine(&client);
  state->run();
  delete state;
  return client;
}
}

TEST(ReadStartLineTester, BasicRequests)
{
  std::string line("GET http://test/ HTTP/1.0\r\n");
  Client client = StateTest(line);
  Request& request = client.getRequest();
  EXPECT_EQ(request.getMethod(), Request::GET);
  EXPECT_EQ(request.getUri().getScheme(), "http");
  EXPECT_EQ(request.getUri().getAuthority(), "//test");
  EXPECT_EQ(request.getUri().getPath(), "/");
  EXPECT_EQ(request.getVersion(), "HTTP/1.0");
}
// NOLINTEND

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
