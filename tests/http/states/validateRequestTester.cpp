
// NOLINTBEGIN

#include "client/Client.hpp"
#include "config/Config.hpp"
#include "config/ServerConfig.hpp"
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
#include <exception>
#include <iostream>
#include <string>

#include <gtest/gtest.h>

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
// ======================= Hostheader Tests ========================
// =================================================================

TEST(ValidateRequestTester, HostheaderMissingIn)
{
  std::string line("GET http://localhost:8080/ HTTP/1.1\r\n\r\n");

  ft::unique_ptr<Client> client = requestTest(line, 8080);

  Response& res = client->getResponse();
  EXPECT_EQ(res.getStatusCode(), StatusCode::BadRequest);
}

// =================================================================
// ======================= XXXXXXXXXX Tests ========================
// =================================================================

TEST(ValidateRequestTester, RootAppend)
{
  std::string line(
    "GET http://localhost:8080/ HTTP/1.1\r\nHost: localhost\r\n\r\n");

  ft::unique_ptr<Client> client = requestTest(line, 8080);

  std::string result = std::string(ASSETS_PATH) + "foo/bar";
  EXPECT_EQ(client->getResource().getPath(), result);
}

// ============================================================================
// Method Tests
// ============================================================================

TEST(ValidateRequestTester, MethodNotAllowed)
{
  std::string line(
    "POST http://methods:8080/ HTTP/1.1\r\nHost: methods\r\n\r\n");

  ft::unique_ptr<Client> client = requestTest(line, 8080);

  Response& res = client->getResponse();
  EXPECT_EQ(res.getStatusCode(), StatusCode::MethodNotAllowed);
}

// ============================================================================
// Decode Tests
// ============================================================================

// TEST(ValidateRequestTester, DecodePercentSpace)
// {
//   ServerConfig conf = createServConf();

//   ft::unique_ptr<Client> client =
//     requestValidate(Request::GET, "/hello%20world/test", conf);

//   const std::string& path = client->getResource().getPath();
//   std::string result = std::string(ASSETS_PATH) + "hello world/test";

//   EXPECT_EQ(path, result);  // create Config for all tests

//   EXPECT_EQ(client->getResponse().getStatusCode(), StatusCode::NotFound);
// }

// TEST(ValidateRequestTester, DecodeAlot)
// {
//   ServerConfig conf = createServConf();

//   ft::unique_ptr<Client> client = requestValidate(
//     Request::GET,
//     "%2F%74%65%73%74%74%68%65%64%65%63%6F%64%65%66%75%6E%63%74%69%6F%6E",
//     conf);

//   const std::string& path = client->getResource().getPath();
//   std::string result = std::string(ASSETS_PATH) + "testthedecodefunction";

//   EXPECT_EQ(path, result);
//   EXPECT_EQ(client->getResponse().getStatusCode(), StatusCode::NotFound);
// }

// TEST(ValidateRequestTester, DecodeEmpty)
// {
//   ServerConfig conf = createServConf();

//   ft::unique_ptr<Client> client = requestValidate(Request::GET, "/te%",
//   conf);

//   EXPECT_EQ(client->getResponse().getStatusCode(), StatusCode::BadRequest);
//   EXPECT_EQ(client->getResource().getType(), Resource::Error);
// }

// TEST(ValidateRequestTester, DecodeInvalid)
// {
//   ServerConfig conf = createServConf();

//   ft::unique_ptr<Client> client =
//     requestValidate(Request::GET, "/te%GGst", conf);

//   EXPECT_EQ(client->getResponse().getStatusCode(), StatusCode::BadRequest);
//   EXPECT_EQ(client->getResource().getType(), Resource::Error);
// }

// TEST(ValidateRequestTester, DecodeInvalidNull)
// {
//   ServerConfig conf = createServConf();

//   ft::unique_ptr<Client> client =
//     requestValidate(Request::GET, "/te%00st", conf);

//   EXPECT_EQ(client->getResponse().getStatusCode(), StatusCode::BadRequest);
//   EXPECT_EQ(client->getResource().getType(), Resource::Error);
// }

// TEST(ValidateRequestTester, DecodeInvalidOnlyOneHex)
// {
//   ServerConfig conf = createServConf();

//   ft::unique_ptr<Client> client = requestValidate(Request::GET, "/te%4",
//   conf);

//   EXPECT_EQ(client->getResponse().getStatusCode(), StatusCode::BadRequest);
//   EXPECT_EQ(client->getResource().getType(), Resource::Error);
// }

// ============================================================================
// Normalize Path Tests
// ============================================================================

// TEST(ValidateRequestTester, NormalizePath)
// {
//   ServerConfig server(Config::getConfig());

//   ft::unique_ptr<Client> client =
//     requestValidate(Request::GET, "/a/b/c/./../../g", server);

//   EXPECT_EQ(client->getResource().getPath(), "/a/g");
// }

// TEST(ValidateRequestTester, NormalizePathMidContent)
// {
//   ServerConfig server(Config::getConfig());

//   ft::unique_ptr<Client> client =
//     requestValidate(Request::GET, "/mid/content=5/../6", server);

//   EXPECT_EQ(client->getResource().getPath(), "/mid/6");
// }

// ============================================================================
// Valid Tests
// ============================================================================

// TEST(ValidateRequestTester, ValidFile)
// {
//   ServerConfig conf = createServConf();
//   std::string file = "valid/minimal.conf";

//   ft::unique_ptr<Client> client = requestValidate(Request::GET, file, conf);

//   EXPECT_EQ(client->getResponse().getStatusCode(), StatusCode::Ok);
// }

// NOLINTEND

// Main function to run all tests
// ss -tulpn | grep 8080
int main(int argc, char** argv)
{
  try {
    const std::string configPath =
      std::string(ASSETS_PATH) + "valid/validateRequest.conf";
    ConfigParser parser(configPath.c_str());
    parser.parseConfig();
    ServerManager::getInstance();
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what();
    return 1;
  }

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
