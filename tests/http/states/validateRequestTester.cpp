#include "client/Client.hpp"
#include "config/Config.hpp"
#include "config/LocationConfig.hpp"
#include "config/ServerConfig.hpp"
#include "http/Request.hpp"
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
                                       config::ServerConfig& serverConfig)
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

TEST(ValidateRequestTester, RootAppend)
{
  config::Config config;
  config::ServerConfig serverConfig(config);
  serverConfig.setRoot("test/");

  ft::unique_ptr<Client> client =
    requestValidate(Request::GET, "/foo/bar", serverConfig);

  EXPECT_EQ(client->getResource().getPath(), "test/foo/bar");
}

TEST(ValidateRequestTester, MethodNotAllowed)
{
  config::Config config;
  config::ServerConfig serverConfig(config);
  serverConfig.setRoot("test/");
  serverConfig.clearAllowedMethods();
  serverConfig.addAllowedMethod("POST");

  ft::unique_ptr<Client> client =
    requestValidate(Request::GET, "/foo/bar", serverConfig);

  EXPECT_EQ(client->getResponse().getStatusCode(), StatusCode::MethodNotAllowed);
}

// NOLINTEND

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
