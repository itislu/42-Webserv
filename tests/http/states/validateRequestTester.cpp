
// NOLINTBEGIN

#include "client/Client.hpp"
#include "event/EventManager.hpp"
#include "http/Resource.hpp"
#include "http/Response.hpp"
#include "http/StatusCode.hpp"
#include "http/states/readRequestLine/ReadRequestLine.hpp"
#include "http/states/validateRequest/ValidateRequest.hpp"
#include "libftpp/memory.hpp"
#include "libftpp/utility.hpp"
#include <config/parser/ConfigParser.hpp>

#include "server/Server.hpp"
#include "server/ServerManager.hpp"
#include "socket/Socket.hpp"
#include "socket/SocketManager.hpp"
#include <iostream>
#include <string>

#include <gtest/gtest.h>
#include <unistd.h>

#ifndef ROOT
#define ROOT "./assets/testWebsite/"
#endif

namespace {

using namespace config;

ft::unique_ptr<Client> requestTest(std::string& rawBuffer, int connection)
{
  ft::unique_ptr<Client> client = ft::make_unique<Client>();
  const Socket& socket = SocketManager::getInstance().getListener(connection);
  const Server* server = ServerManager::getInstance().getInitServer(socket);
  client->setServer(server);
  client->setSocket(&socket);
  client->getStateHandler().setState<ReadRequestLine>();
  client->getInBuff().append(rawBuffer);

  while (!client->getStateHandler().isDone()) {
    ValidateRequest* statePtr =
      dynamic_cast<ValidateRequest*>(client->getStateHandler().getState());
    client->getStateHandler().getState()->run();
    if (statePtr != FT_NULLPTR) {
      break;
    }
  }
  return ft::move(client);
}

} // namespace

// =================================================================
// Hostheader Tests
// =================================================================

TEST(ValidateRequestTester, ValidHostHeader)
{
  std::string line("GET http://serv01:8080/ HTTP/1.1\r\nHost: serv01\r\n\r\n");

  ft::unique_ptr<Client> client = requestTest(line, 8080);

  Resource& source = client->getResource();
  std::string result = std::string(ROOT) + "serv01.html";

  EXPECT_EQ(source.getPath(), result);
}

TEST(ValidateRequestTester, HostheaderMissingInHttp1_1)
{
  std::string line("GET http://localhost:8080/ HTTP/1.1\r\n\r\n");

  ft::unique_ptr<Client> client = requestTest(line, 8080);

  Response& res = client->getResponse();
  EXPECT_EQ(res.getStatusCode(), StatusCode::BadRequest);
}

TEST(ValidateRequestTester, HostHeaderNoUriHost)
{
  std::string line("GET /index.html HTTP/1.1\r\nHost: localhost:8080\r\n\r\n");

  ft::unique_ptr<Client> client = requestTest(line, 8080);

  Resource& source = client->getResource();
  std::string result = std::string(ROOT) + "index.html";

  EXPECT_EQ(source.getPath(), result);
}

TEST(ValidateRequestTester, UriHostHasPrio)
{
  std::string line("GET http://serv01:8080/ HTTP/1.1\r\nHost: serv02:8080\r\n\r\n");

  ft::unique_ptr<Client> client = requestTest(line, 8080);

  Resource& source = client->getResource();
  std::string result = std::string(ROOT) + "serv01.html";

  EXPECT_EQ(source.getPath(), result);
}

TEST(ValidateRequestTester, UriHostHasPrio02)
{
  std::string line("GET http://serv02:8080/ HTTP/1.1\r\nHost: serv01:8080\r\n\r\n");

  ft::unique_ptr<Client> client = requestTest(line, 8080);

  Resource& source = client->getResource();
  std::string result = std::string(ROOT) + "serv02.html";

  EXPECT_EQ(source.getPath(), result);
}

TEST(ValidateRequestTester, UriHostHasPrio03)
{
  std::string line(
    "GET http://localhost:8080/ HTTP/1.1\r\nHost: SomeOtherHost:8080\r\n\r\n");

  ft::unique_ptr<Client> client = requestTest(line, 8080);

  Resource& source = client->getResource();
  std::string result = std::string(ROOT) + "serv01.html";

  EXPECT_EQ(source.getPath(), result);
}

TEST(ValidateRequestTester, SlashInHostHeader)
{
  std::string line(
    "GET http://serv02:8080/ HTTP/1.1\r\nHost: serv02/alarm:8080\r\n\r\n");

  ft::unique_ptr<Client> client = requestTest(line, 8080);

  Response& response = client->getResponse();

  EXPECT_EQ(response.getStatusCode(), StatusCode::BadRequest);
}

TEST(ValidateRequestTester, MatchCaseInsensitive)
{
  std::string line("GET / HTTP/1.1\r\nHost: SERV02:8080\r\n\r\n");

  ft::unique_ptr<Client> client = requestTest(line, 8080);

  Resource& source = client->getResource();
  std::string result = std::string(ROOT) + "serv02.html";

  EXPECT_EQ(source.getPath(), result);
}

// =================================================================
// Append Root Tests
// =================================================================

TEST(ValidateRequestTester, RootAppend)
{
  std::string line("GET /foo/bar HTTP/1.1\r\nHost: localhost:8080\r\n\r\n");

  ft::unique_ptr<Client> client = requestTest(line, 8080);

  Resource& source = client->getResource();
  std::string result = std::string(ROOT) + "foo/bar";

  EXPECT_EQ(source.getPath(), result);
  EXPECT_EQ(client->getResponse().getStatusCode(), StatusCode::NotFound);
}

// ============================================================================
// Method Tests
// ============================================================================

TEST(ValidateRequestTester, MethodNotAllowedPost)
{
  std::string line(
    "POST http://methods:8080/ HTTP/1.1\r\nHost: methods\r\n\r\n");

  ft::unique_ptr<Client> client = requestTest(line, 8080);

  Response& res = client->getResponse();
  EXPECT_EQ(res.getStatusCode(), StatusCode::MethodNotAllowed);
}

TEST(ValidateRequestTester, MethodNotAllowedDelete)
{
  std::string line(
    "DELETE http://methods:8080/ HTTP/1.1\r\nHost: methods\r\n\r\n");

  ft::unique_ptr<Client> client = requestTest(line, 8080);

  Response& res = client->getResponse();
  EXPECT_EQ(res.getStatusCode(), StatusCode::MethodNotAllowed);
}

// ============================================================================
// Decode Tests
// ============================================================================

TEST(ValidateRequestTester, DecodePercentSpace)
{
  std::string line("GET http://localhost:8080/hello%20world/test "
                   "HTTP/1.1\r\nHost: serv1\r\n\r\n");
  ft::unique_ptr<Client> client = requestTest(line, 8080);

  const std::string& path = client->getResource().getPath();
  Response& response = client->getResponse();
  std::string result = std::string(ROOT) + "hello world/test";

  EXPECT_EQ(path, result);
  EXPECT_EQ(response.getStatusCode(), StatusCode::NotFound);
}

TEST(ValidateRequestTester, DecodeAlot)
{
  std::string line("GET "
                   "http://localhost:8080/"
                   "%74%65%73%74%64%65%63%6F%64%65 "
                   "HTTP/1.1\r\nHost: serv1\r\n\r\n");
  ft::unique_ptr<Client> client = requestTest(line, 8080);

  const std::string& path = client->getResource().getPath();
  Response& response = client->getResponse();
  std::string result = std::string(ROOT) + "testdecode";

  EXPECT_EQ(path, result);
  EXPECT_EQ(response.getStatusCode(), StatusCode::NotFound);
}

TEST(ValidateRequestTester, DecodeEmpty)
{
  std::string line(
    "GET http://localhost:8080/te% HTTP/1.1\r\nHost: localhost:8080\r\n\r\n");

  ft::unique_ptr<Client> client = requestTest(line, 8080);

  EXPECT_EQ(client->getResponse().getStatusCode(), StatusCode::BadRequest);
}

TEST(ValidateRequestTester, DecodeInvalid)
{

  std::string line("GET http://localhost:8080/te%GGst HTTP/1.1\r\nHost: "
                   "localhost:8080\r\n\r\n");
  ft::unique_ptr<Client> client = requestTest(line, 8080);

  EXPECT_EQ(client->getResponse().getStatusCode(), StatusCode::BadRequest);
}

TEST(ValidateRequestTester, DecodeInvalidNull)
{
  std::string line("GET http://localhost:8080/te%00st HTTP/1.1\r\nHost: "
                   "localhost:8080\r\n\r\n");
  ft::unique_ptr<Client> client = requestTest(line, 8080);

  EXPECT_EQ(client->getResponse().getStatusCode(), StatusCode::BadRequest);
  EXPECT_EQ(client->getResource().getType(), Resource::Error);
}

TEST(ValidateRequestTester, DecodeOnlyOneHex)
{
  std::string line("GET http://localhost:8080/te%4 HTTP/1.1\r\nHost: "
                   "localhost:8080\r\n\r\n");
  ft::unique_ptr<Client> client = requestTest(line, 8080);

  EXPECT_EQ(client->getResponse().getStatusCode(), StatusCode::BadRequest);
}

// NOLINTEND

// Main function to run all tests
// ss -tulpn | grep 8080
#include <unistd.h>
#include <limits.h> // For PATH_MAX

int main(int argc, char** argv)
{
  if (chdir(PROJECT_ROOT) == -1) {
    std::cerr << "Failed to change directory to: " << PROJECT_ROOT << "\n";
    return 1;
  }

    std::string configPath = std::string(ASSETS_PATH) + "valid/validateRequest.conf";
    std::cout << "[DEBUG] Attempting to open: " << configPath << std::endl;

    ConfigParser parser(configPath.c_str());
  parser.parseConfig();

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
