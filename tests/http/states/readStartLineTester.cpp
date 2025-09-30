
#include "client/Client.hpp"
#include "http/states/IState.hpp"
#include "http/Request.hpp"
#include "http/states/IState.hpp"
#include "http/states/ReadStartLine.hpp"
#include <gtest/gtest.h>
#include <string>

static const std::string TESTER_NAME = "ReadStartLineTester";

Client StateTest(std::string startLine)
{
  Client client =  Client(1);
  client.inBuffAddStr(startLine);
  IState* state = new ReadStartLine();
  state->run(client);
  return client;
}

TEST(TESTER_NAME, BasicRequests)
{
  Client client = StateTest("GET test/ HTTP/1.0\r\n");
  Request& request = client.getRequest();
  EXPECT_EQ(request.getMethod(), request::GET);
  EXPECT_EQ(request.getUri().getRaw(), "test/");
  EXPECT_EQ(request.getVersion(), "HTTP/1.0");
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
