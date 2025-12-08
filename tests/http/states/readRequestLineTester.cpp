#include "http/states/readRequestLine/ParseUri.hpp"
#include <client/Client.hpp>
#include <http/Request.hpp>
#include <http/Response.hpp>
#include <http/StatusCode.hpp>
#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <testUtils.hpp>
#include <utils/state/IState.hpp>

#include <gtest/gtest.h>
#include <string>

using testUtils::makeString;

// NOLINTBEGIN

namespace {

ft::unique_ptr<Client> StateTest(const std::string& requestLine)
{
  ft::unique_ptr<Client> client = ft::make_unique<Client>();
  client->getInBuff().append(requestLine);
  client->getStateHandler().setState<ReadRequestLine>();
  client->getStateHandler().getState()->run();
  return ft::move(client);
}

} // namespace

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
                   "http:/?www.example.org?pub/WWW/TheProject.html?test?frag "
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
                   "http:/ "
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
  EXPECT_EQ(request.getVersion(), "HTTP/1.1");
}

// TODO Enable when bad request handling is implemented.
TEST(ReadRequestLineTester, DISABLED_QueryBadRequest)
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
  std::string line("DELETEA "
                   "//www.example.org "
                   "HTTP/1.1\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response = client->getResponse();

  EXPECT_EQ(response.getStatusCode().getCode(), StatusCode::NotImplemented);
}

TEST(ReadRequestLineTester, UriTooLong)
{
  {
    ParseUri::setMaxUriLength(10);
    std::string line("GET /123456789 HTTP/1.1\r\n");
    ft::unique_ptr<Client> client = StateTest(line);
    Response& response = client->getResponse();
    EXPECT_EQ(response.getStatusCode().getCode(), StatusCode::UriTooLong);
  }
  {
    ParseUri::setMaxUriLength(11);
    std::string line("GET /123456789 HTTP/1.1\r\n");
    ft::unique_ptr<Client> client = StateTest(line);
    Response& response = client->getResponse();
    EXPECT_EQ(response.getStatusCode().getCode(), StatusCode::Ok);
  }

  ParseUri::resetMaxUriLength();
}

TEST(ReadRequestLineTester, NulByteInMethod)
{
  std::string line = makeString("GE\0T /path HTTP/1.1\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response = client->getResponse();

  EXPECT_EQ(response.getStatusCode().getCode(), StatusCode::BadRequest);
}

TEST(ReadRequestLineTester, NulByteInUri)
{
  std::string line = makeString("GET /pa\0th HTTP/1.1\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response = client->getResponse();

  EXPECT_EQ(response.getStatusCode().getCode(), StatusCode::BadRequest);
}

TEST(ReadRequestLineTester, NulByteInVersion)
{
  std::string line = makeString("GET /path HTTP/1.\0001\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response = client->getResponse();

  EXPECT_EQ(response.getStatusCode().getCode(), StatusCode::BadRequest);
}

TEST(ReadRequestLineTester, NulByteAtStartOfRequestLine)
{
  std::string line = makeString("\0GET /path HTTP/1.1\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response = client->getResponse();

  EXPECT_EQ(response.getStatusCode().getCode(), StatusCode::BadRequest);
}

TEST(ReadRequestLineTester, MultipleNulBytes)
{
  // Multiple NUL bytes in request line should be rejected
  std::string line = makeString("GET\0 /pa\0th HTTP/1.1\0\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response = client->getResponse();

  EXPECT_EQ(response.getStatusCode().getCode(), StatusCode::BadRequest);
}

TEST(ReadRequestLineTester, InvalidVersion)
{
  std::string line("GET "
                   "http:/ "
                   "HTTP/2.0\r\n");
  ft::unique_ptr<Client> client = StateTest(line);
  Response& response = client->getResponse();
  EXPECT_EQ(response.getStatusCode().getCode(),
            StatusCode::HttpVersionNotSupported);
}

// NOLINTEND

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
