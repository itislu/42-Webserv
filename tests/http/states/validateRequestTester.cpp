#include "config/Config.hpp"
#include "config/ServerConfig.hpp"
#include "http/Response.hpp"
#include "http/StatusCode.hpp"
#include "http/states/validateRequest/ValidateRequest.hpp"
#include "server/Server.hpp"
#include "socket/Socket.hpp"
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

ft::unique_ptr<Client> requestTest(std::string& rawBuffer)
{
  ServerConfig serverConfig(Config::getConfig());
  const Server server(serverConfig);
  Socket socket(8080);
  ft::unique_ptr<Client> client = ft::make_unique<Client>();
  client->setServer(&server);
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

TEST(ValidateRequestHost, HostheaderMissingIn)
{
  std::string line("GET http://localhost:8080/ HTTP/1.1\r\n\r\n");
  ft::unique_ptr<Client> client = requestTest(line);

  Response& res = client->getResponse();
  EXPECT_EQ(res.getStatusCode(), StatusCode::BadRequest);
}

// NOLINTEND

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
