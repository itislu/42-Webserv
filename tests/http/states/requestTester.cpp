#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Request.hpp>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>

#include <cstddef>
#include <gtest/gtest.h>
#include <string>

// NOLINTBEGIN

namespace {

/**
 * Test with incomplete buffer
 * The buffer is extended by one char until parsing is done
 */
ft::unique_ptr<Client> requestTestCharByChar(std::string& rawBuffer)
{
  ft::unique_ptr<Client> client = ft::make_unique<Client>();
  client->getStateHandler().setState<ReadRequestLine>();

  std::size_t index = 0;
  while (!client->getStateHandler().isDone() && index < rawBuffer.size()) {
    std::string chr;
    chr.push_back(rawBuffer[index]);
    client->getInBuff().add(chr);
    client->getStateHandler().getState()->run();
    index++;
  }
  return ft::move(client);
}

/**
 * Test with complete buffer
 */
ft::unique_ptr<Client> requestTest(std::string& rawBuffer)
{
  ft::unique_ptr<Client> client = ft::make_unique<Client>();
  client->getStateHandler().setState<ReadRequestLine>();
  client->getInBuff().add(rawBuffer);

  while (!client->getStateHandler().isDone()) {
    client->getStateHandler().getState()->run();
  }
  return ft::move(client);
}
}

TEST(RequestTester, PartialBufferTest)
{
  std::string line("GET http://www.example.com/test/index.html HTTP/1.0\r\n"
                   "Host: webserv\r\n"
                   "TestHeader: 7\r\n"
                   "\r\n");
  ft::unique_ptr<Client> client = requestTestCharByChar(line);
  Request& request = client->getRequest();

  EXPECT_EQ(request.getMethod(), Request::GET);
  EXPECT_EQ(request.getUri().getScheme(), "http");
  EXPECT_EQ(request.getUri().getAuthority().getHost(), "www.example.com");
  EXPECT_EQ(request.getUri().getPath(), "/test/index.html");
  EXPECT_EQ(request.getVersion(), "HTTP/1.0");

  Headers& headers = request.getHeaders();
  EXPECT_NO_THROW(EXPECT_EQ(headers.at("Host"), "webserv"));
  EXPECT_NO_THROW(EXPECT_EQ(headers.at("TestHeader"), "7"));
}

TEST(RequestTester, CompleteBufferTest)
{
  std::string line("GET http://www.example.com/test/index.html HTTP/1.0\r\n"
                   "Host: webserv\r\n"
                   "TestHeader: 7\r\n"
                   "\r\n");
  ft::unique_ptr<Client> client = requestTest(line);
  Request& request = client->getRequest();

  EXPECT_EQ(request.getMethod(), Request::GET);
  EXPECT_EQ(request.getUri().getScheme(), "http");
  EXPECT_EQ(request.getUri().getAuthority().getHost(), "www.example.com");
  EXPECT_EQ(request.getUri().getPath(), "/test/index.html");
  EXPECT_EQ(request.getVersion(), "HTTP/1.0");

  Headers& headers = request.getHeaders();
  EXPECT_NO_THROW(EXPECT_EQ(headers.at("Host"), "webserv"));
  EXPECT_NO_THROW(EXPECT_EQ(headers.at("TestHeader"), "7"));
}

// NOLINTEND

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
