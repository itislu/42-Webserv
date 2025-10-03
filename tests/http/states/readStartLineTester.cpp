
#include "client/Client.hpp"
#include "http/Request.hpp"
#include "http/states/IState.hpp"
#include "http/states/ReadStartLine.hpp"
#include <gtest/gtest.h>
#include <string>

namespace {
Client StateTest(std::string& startLine)
{
  Client client = Client();
  client.getInBuff().add(startLine);
  IState* state = new ReadStartLine(&client);
  state->run();
  delete state;
  return client;
}
}


// NOLINTBEGIN
TEST(ReadStartLineTester, BasicRequests)
{
  std::string line("GET test/ HTTP/1.0\r\n");
  Client client = StateTest(line);
  Request& request = client.getRequest();
  EXPECT_EQ(request.getMethod(), Request::GET);
  EXPECT_EQ(request.getUri().getRaw(), "test/");
  EXPECT_EQ(request.getVersion(), "HTTP/1.0");
}
// NOLINTEND

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}