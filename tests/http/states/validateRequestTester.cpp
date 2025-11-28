#include "client/Client.hpp"
#include "config/Config.hpp"
#include "config/LocationConfig.hpp"
#include "config/ServerConfig.hpp"
#include "http/Request.hpp"
#include "http/Resource.hpp"
#include "http/StatusCode.hpp"
#include "http/Uri.hpp"
#include "http/states/validateRequest/ValidateRequest.hpp"
#include "libftpp/memory.hpp"
#include "libftpp/utility.hpp"
#include "server/Server.hpp"
#include <gtest/gtest.h>
#include <string>

// NOLINTBEGIN

using config::Config;
using config::LocationConfig;
using config::ServerConfig;

ft::unique_ptr<Client> requestValidate(Request::Method method,
                                       const std::string& uriPath,
                                       ServerConfig& serverConfig)
{
  Uri uri;
  const Server server(serverConfig);
  ft::unique_ptr<Client> client = ft::make_unique<Client>();
  client->setServer(&server);

  // populate parsed request
  client->getRequest().setMethod(method);
  uri.setPath(uriPath);
  client->getRequest().setUri(uri);

  client->getStateHandler().setState<ValidateRequest>();

  // run ValidateRequest alone
  while (!client->getStateHandler().isDone()) {
    client->getStateHandler().getState()->run();
  }

  return ft::move(client);
}

ServerConfig createServConf()
{
  config::Config config;
  config::ServerConfig server(config);
  server.setRoot(ASSETS_PATH);

  return server;
}

TEST(ValidateRequestTester, RootAppend)
{
  ServerConfig conf = createServConf();

  ft::unique_ptr<Client> client =
    requestValidate(Request::GET, "/foo/bar", conf);

  std::string result = std::string(ASSETS_PATH) + "foo/bar";
  EXPECT_EQ(client->getResource().getPath(), result);
}

// ============================================================================
// Method Tests
// ============================================================================

TEST(ValidateRequestTester, MethodNotAllowed)
{
  ServerConfig conf = createServConf();
  conf.clearAllowedMethods();
  conf.addAllowedMethod("POST");

  ft::unique_ptr<Client> client =
    requestValidate(Request::GET, "/foo/bar", conf);

  EXPECT_EQ(client->getResponse().getStatusCode(),
            StatusCode::MethodNotAllowed);
  EXPECT_EQ(client->getResource().getType(), Resource::Error);
}

// ============================================================================
// Decode Tests
// ============================================================================

TEST(ValidateRequestTester, DecodePercentSpace)
{
  ServerConfig conf = createServConf();

  ft::unique_ptr<Client> client =
    requestValidate(Request::GET, "/hello%20world/test", conf);

  const std::string& path = client->getResource().getPath();
  std::string result = std::string(ASSETS_PATH) + "hello world/test";

  EXPECT_EQ(path, result);
  EXPECT_EQ(client->getResponse().getStatusCode(), StatusCode::NotFound);
}

TEST(ValidateRequestTester, DecodeAlot)
{
  ServerConfig conf = createServConf();

  ft::unique_ptr<Client> client = requestValidate(
    Request::GET,
    "%2F%74%65%73%74%74%68%65%64%65%63%6F%64%65%66%75%6E%63%74%69%6F%6E",
    conf);

  const std::string& path = client->getResource().getPath();
  std::string result = std::string(ASSETS_PATH) + "testthedecodefunction";

  EXPECT_EQ(path, result);
  EXPECT_EQ(client->getResponse().getStatusCode(), StatusCode::NotFound);
}

TEST(ValidateRequestTester, DecodeEmpty)
{
  ServerConfig conf = createServConf();

  ft::unique_ptr<Client> client = requestValidate(Request::GET, "/te%", conf);

  EXPECT_EQ(client->getResponse().getStatusCode(), StatusCode::BadRequest);
  EXPECT_EQ(client->getResource().getType(), Resource::Error);
}

TEST(ValidateRequestTester, DecodeInvalid)
{
  ServerConfig conf = createServConf();

  ft::unique_ptr<Client> client =
    requestValidate(Request::GET, "/te%GGst", conf);

  EXPECT_EQ(client->getResponse().getStatusCode(), StatusCode::BadRequest);
  EXPECT_EQ(client->getResource().getType(), Resource::Error);
}

TEST(ValidateRequestTester, DecodeInvalidNull)
{
  ServerConfig conf = createServConf();

  ft::unique_ptr<Client> client =
    requestValidate(Request::GET, "/te%00st", conf);

  EXPECT_EQ(client->getResponse().getStatusCode(), StatusCode::BadRequest);
  EXPECT_EQ(client->getResource().getType(), Resource::Error);
}

TEST(ValidateRequestTester, DecodeInvalidOnlyOneHex)
{
  ServerConfig conf = createServConf();

  ft::unique_ptr<Client> client = requestValidate(Request::GET, "/te%4", conf);

  EXPECT_EQ(client->getResponse().getStatusCode(), StatusCode::BadRequest);
  EXPECT_EQ(client->getResource().getType(), Resource::Error);
}

// ============================================================================
// Normalize Path Tests
// ============================================================================

TEST(ValidateRequestTester, NormalizePath)
{
  Config config;
  ServerConfig server(config);

  ft::unique_ptr<Client> client =
    requestValidate(Request::GET, "/a/b/c/./../../g", server);

  EXPECT_EQ(client->getResource().getPath(), "/a/g");
}

TEST(ValidateRequestTester, NormalizePath01)
{
  Config config;
  ServerConfig server(config);
  server.setRoot("server_root/");

  ft::unique_ptr<Client> client =
    requestValidate(Request::GET, "/a/../.././..//////..//", server);

  EXPECT_EQ(client->getResource().getPath(), "server_root/");
}

TEST(ValidateRequestTester, NormalizePathMidContent)
{
  Config config;
  ServerConfig server(config);

  ft::unique_ptr<Client> client =
    requestValidate(Request::GET, "/mid/content=5/../6", server);

  EXPECT_EQ(client->getResource().getPath(), "/mid/6");
}

// ============================================================================
// Valid Tests
// ============================================================================

TEST(ValidateRequestTester, ValidFile)
{
  ServerConfig conf = createServConf();
  std::string file = "valid/minimal.conf";

  ft::unique_ptr<Client> client = requestValidate(Request::GET, file, conf);

  EXPECT_EQ(client->getResponse().getStatusCode(), StatusCode::Ok);
}

// NOLINTEND

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
