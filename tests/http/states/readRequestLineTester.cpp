#include "http/states/readRequestLine/ParseUri.hpp"
#include <client/Client.hpp>
#include <http/Request.hpp>
#include <http/Response.hpp>
#include <http/StatusCode.hpp>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/state/IState.hpp>

#include <gtest/gtest.h>
#include <string>

// NOLINTBEGIN

namespace {
ft::unique_ptr<Client> StateTest(const std::string& requestLine)
{
  ft::unique_ptr<Client> client = ft::make_unique<Client>();
  client->getInBuff().add(requestLine);
  client->getStateHandler().setState<ReadRequestLine>();
  client->getStateHandler().getState()->run();
  return ft::move(client);
}
}

TEST(ReadRequestLineTester, BasicRequests)
{
  std::string line("GET http://www.example.com/test/test.txt HTTP/1.0\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Request& request = client->getRequest();
  EXPECT_EQ(request.getMethod(), Request::GET);
  EXPECT_EQ(request.getUri().getScheme(), "http");
  EXPECT_EQ(request.getUri().getAuthority().getHost(), "www.example.com");
  EXPECT_EQ(request.getUri().getPath(), "/test/test.txt");
  EXPECT_EQ(request.getVersion(), "HTTP/1.0");
}

TEST(ReadRequestLineTester, OriginForm)
{
  std::string line("GET /where?test HTTP/1.1\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Request& request = client->getRequest();
  EXPECT_EQ(request.getMethod(), Request::GET);
  EXPECT_EQ(request.getUri().getScheme(), "");
  EXPECT_EQ(request.getUri().getAuthority().getHost(), "");
  EXPECT_EQ(request.getUri().getPath(), "/where");
  EXPECT_EQ(request.getUri().getQuery(), "test");
  EXPECT_EQ(request.getVersion(), "HTTP/1.1");
}

TEST(ReadRequestLineTester, AbsoluteForm)
{
  std::string line("GET "
                   "http://www.example.org/pub/WWW/TheProject.html?test "
                   "HTTP/1.1\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Request& request = client->getRequest();
  EXPECT_EQ(request.getMethod(), Request::GET);
  EXPECT_EQ(request.getUri().getScheme(), "http");
  EXPECT_EQ(request.getUri().getAuthority().getHost(), "www.example.org");
  EXPECT_EQ(request.getUri().getPath(), "/pub/WWW/TheProject.html");
  EXPECT_EQ(request.getUri().getQuery(), "test");
  EXPECT_EQ(request.getUri().getFragment(), "");
  EXPECT_EQ(request.getVersion(), "HTTP/1.1");
}

TEST(ReadRequestLineTester, SchemeAuthorityQuery)
{
  std::string line("GET "
                   "http://www.example.org?pub/WWW/TheProject.html?test "
                   "HTTP/1.1\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Request& request = client->getRequest();
  EXPECT_EQ(request.getMethod(), Request::GET);
  EXPECT_EQ(request.getUri().getScheme(), "http");
  EXPECT_EQ(request.getUri().getAuthority().getHost(), "www.example.org");
  EXPECT_EQ(request.getUri().getPath(), "");
  EXPECT_EQ(request.getUri().getQuery(), "pub/WWW/TheProject.html?test");
  EXPECT_EQ(request.getVersion(), "HTTP/1.1");
}

TEST(ReadRequestLineTester, SchemePathQuery)
{
  std::string line("GET "
                   "http:///www.example.org?pub/WWW/TheProject.html?test "
                   "HTTP/1.1\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Request& request = client->getRequest();
  EXPECT_EQ(request.getMethod(), Request::GET);
  EXPECT_EQ(request.getUri().getScheme(), "http");
  EXPECT_EQ(request.getUri().getAuthority().getHost(), "");
  EXPECT_EQ(request.getUri().getPath(), "/www.example.org");
  EXPECT_EQ(request.getUri().getQuery(), "pub/WWW/TheProject.html?test");
  EXPECT_EQ(request.getVersion(), "HTTP/1.1");
}

TEST(ReadRequestLineTester, SchemeQuery)
{
  std::string line("GET "
                   "http://?www.example.org?pub/WWW/TheProject.html?test "
                   "HTTP/1.1\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Request& request = client->getRequest();
  EXPECT_EQ(request.getMethod(), Request::GET);
  EXPECT_EQ(request.getUri().getScheme(), "http");
  EXPECT_EQ(request.getUri().getAuthority().getHost(), "");
  EXPECT_EQ(request.getUri().getPath(), "");
  EXPECT_EQ(request.getUri().getQuery(),
            "www.example.org?pub/WWW/TheProject.html?test");
  EXPECT_EQ(request.getVersion(), "HTTP/1.1");
}

TEST(ReadRequestLineTester, SchemePathQuery1)
{
  std::string line("GET "
                   "http://?www.example.org?pub/WWW/TheProject.html?test?frag "
                   "HTTP/1.1\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Request& request = client->getRequest();
  EXPECT_EQ(request.getMethod(), Request::GET);
  EXPECT_EQ(request.getUri().getScheme(), "http");
  EXPECT_EQ(request.getUri().getAuthority().getHost(), "");
  EXPECT_EQ(request.getUri().getPath(), "");
  EXPECT_EQ(request.getUri().getQuery(),
            "www.example.org?pub/WWW/TheProject.html?test?frag");
  EXPECT_EQ(request.getUri().getFragment(), "");
  EXPECT_EQ(request.getVersion(), "HTTP/1.1");
}

TEST(ReadRequestLineTester, SchemePath)
{
  std::string line("GET "
                   "http:// "
                   "HTTP/1.1\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Request& request = client->getRequest();
  EXPECT_EQ(request.getMethod(), Request::GET);
  EXPECT_EQ(request.getUri().getScheme(), "http");
  EXPECT_EQ(request.getUri().getAuthority().getHost(), "");
  EXPECT_EQ(request.getUri().getPath(), "");
  EXPECT_EQ(request.getUri().getQuery(), "");
  EXPECT_EQ(request.getUri().getFragment(), "");
  EXPECT_EQ(request.getVersion(), "HTTP/1.1");
}

TEST(ReadRequestLineTester, SchemeQuery1)
{
  std::string line("GET "
                   "http://?www.example.org?pub/WWW/TheProject.html?test?frag "
                   "HTTP/1.1\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Request& request = client->getRequest();
  EXPECT_EQ(request.getMethod(), Request::GET);
  EXPECT_EQ(request.getUri().getScheme(), "http");
  EXPECT_EQ(request.getUri().getAuthority().getHost(), "");
  EXPECT_EQ(request.getUri().getPath(), "");
  EXPECT_EQ(request.getUri().getQuery(),
            "www.example.org?pub/WWW/TheProject.html?test?frag");
  EXPECT_EQ(request.getUri().getFragment(), "");
  EXPECT_EQ(request.getVersion(), "HTTP/1.1");
}

TEST(ReadRequestLineTester, Scheme)
{
  std::string line("GET "
                   "http:// "
                   "HTTP/1.1\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Request& request = client->getRequest();
  EXPECT_EQ(request.getMethod(), Request::GET);
  EXPECT_EQ(request.getUri().getScheme(), "http");
  EXPECT_EQ(request.getUri().getAuthority().getHost(), "");
  EXPECT_EQ(request.getUri().getPath(), "");
  EXPECT_EQ(request.getUri().getQuery(), "");
  EXPECT_EQ(request.getUri().getFragment(), "");
  EXPECT_EQ(request.getVersion(), "HTTP/1.1");
}

TEST(ReadRequestLineTester, PathQuery)
{
  std::string line("GET "
                   "/www.example.org/pub?WWW/TheProject.html?test "
                   "HTTP/1.1\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Request& request = client->getRequest();
  EXPECT_EQ(request.getMethod(), Request::GET);
  EXPECT_EQ(request.getUri().getScheme(), "");
  EXPECT_EQ(request.getUri().getAuthority().getHost(), "");
  EXPECT_EQ(request.getUri().getPath(), "/www.example.org/pub");
  EXPECT_EQ(request.getUri().getQuery(), "WWW/TheProject.html?test");
  EXPECT_EQ(request.getVersion(), "HTTP/1.1");
}

TEST(ReadRequestLineTester, Path1)
{
  std::string line("GET "
                   "/www.example.org "
                   "HTTP/1.1\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Request& request = client->getRequest();
  EXPECT_EQ(request.getMethod(), Request::GET);
  EXPECT_EQ(request.getUri().getScheme(), "");
  EXPECT_EQ(request.getUri().getAuthority().getHost(), "");
  EXPECT_EQ(request.getUri().getPath(), "/www.example.org");
  EXPECT_EQ(request.getUri().getQuery(), "");
  EXPECT_EQ(request.getUri().getFragment(), "");
  EXPECT_EQ(request.getVersion(), "HTTP/1.1");
}

TEST(ReadRequestLineTester, Path2)
{
  std::string line("GET "
                   "//www.example.org "
                   "HTTP/1.1\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Request& request = client->getRequest();
  EXPECT_EQ(request.getMethod(), Request::GET);
  EXPECT_EQ(request.getUri().getScheme(), "");
  EXPECT_EQ(request.getUri().getAuthority().getHost(), "");
  EXPECT_EQ(request.getUri().getPath(), "//www.example.org");
  EXPECT_EQ(request.getUri().getQuery(), "");
  EXPECT_EQ(request.getUri().getFragment(), "");
  EXPECT_EQ(request.getVersion(), "HTTP/1.1");
}

TEST(ReadRequestLineTester, Path3)
{
  std::string line("GET "
                   "///www.example.org "
                   "HTTP/1.1\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Request& request = client->getRequest();
  EXPECT_EQ(request.getMethod(), Request::GET);
  EXPECT_EQ(request.getUri().getScheme(), "");
  EXPECT_EQ(request.getUri().getAuthority().getHost(), "");
  EXPECT_EQ(request.getUri().getPath(), "///www.example.org");
  EXPECT_EQ(request.getUri().getQuery(), "");
  EXPECT_EQ(request.getUri().getFragment(), "");
  EXPECT_EQ(request.getVersion(), "HTTP/1.1");
}

TEST(ReadRequestLineTester, QueryBadRequest)
{
  std::string line("GET "
                   "?query//www.example.org "
                   "HTTP/1.1\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response = client->getResponse();

  EXPECT_EQ(response.getStatusCode().getCode(), StatusCode::BadRequest);
}

TEST(ReadRequestLineTester, NotImplemented)
{
  std::string line("TOLONGMETHOD "
                   "http://auth/path/ "
                   "HTTP/1.1\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response = client->getResponse();

  EXPECT_EQ(response.getStatusCode().getCode(), StatusCode::NotImplemented);
}

/**
 * only the request-target is checked at the moment
 * if Method is to long we will get StatusCode NotImplemented
 * Version has no variable length anyway
 */
TEST(ReadRequestLineTester, UriTooLong)
{
  std::string line("GET "
                   "/www.example.org/pub?WWW/ "
                   "HTTP/1.1\r\n");

  // Too long
  ParseUri::setMaxUriLength(25);
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response1 = client->getResponse();
  EXPECT_EQ(response1.getStatusCode().getCode(), StatusCode::UriTooLong);

  // Not too long
  ParseUri::setMaxUriLength(26);
  ft::unique_ptr<Client> client1 = StateTest(line);
  Response& response2 = client1->getResponse();
  EXPECT_NE(response2.getStatusCode().getCode(), StatusCode::UriTooLong);

  ParseUri::setMaxUriLength(ParseUri::defaultMaxUriLength);
}

// NOLINTEND

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
