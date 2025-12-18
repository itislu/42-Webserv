#include <utils/convert.hpp>

#include <arpa/inet.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>

namespace {

std::string convertIPv4Address(const char* ipStr)
{
  sockaddr_storage addr = {};
  addr.ss_family = AF_INET;

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  sockaddr_in& ipv4 = reinterpret_cast<sockaddr_in&>(addr);
  if (inet_pton(AF_INET, ipStr, &ipv4.sin_addr) != 1) {
    throw std::runtime_error(std::string("Failed to parse IPv4 address: ") +
                             ipStr);
  }

  return utils::addrToString(addr);
}

std::string convertIPv6Address(const char* ipStr)
{
  sockaddr_storage addr = {};
  addr.ss_family = AF_INET6;

  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  sockaddr_in6& ipv6 = reinterpret_cast<sockaddr_in6&>(addr);
  if (inet_pton(AF_INET6, ipStr, &ipv6.sin6_addr) != 1) {
    throw std::runtime_error(std::string("Failed to parse IPv6 address: ") +
                             ipStr);
  }

  return utils::addrToString(addr);
}

} // namespace

TEST(AddrToStringTester, IPv4Localhost)
{
  const char* const ipStr = "127.0.0.1";
  EXPECT_EQ(convertIPv4Address(ipStr), ipStr);
}

TEST(AddrToStringTester, IPv4AllZeros)
{
  const char* const ipStr = "0.0.0.0";
  EXPECT_EQ(convertIPv4Address(ipStr), ipStr);
}

TEST(AddrToStringTester, IPv4AllOnes)
{
  const char* const ipStr = "255.255.255.255";
  EXPECT_EQ(convertIPv4Address(ipStr), ipStr);
}

TEST(AddrToStringTester, IPv4Realistic)
{
  const char* const ipStr = "213.164.21.206";
  EXPECT_EQ(convertIPv4Address(ipStr), ipStr);
}

TEST(AddrToStringTester, IPv6Localhost)
{
  const char* const ipStr = "0:0:0:0:0:0:0:1";
  EXPECT_EQ(convertIPv6Address(ipStr), ipStr);
}

TEST(AddrToStringTester, IPv6AllZeros)
{
  const char* const ipStr = "0:0:0:0:0:0:0:0";
  EXPECT_EQ(convertIPv6Address(ipStr), ipStr);
}

TEST(AddrToStringTester, IPv6AllOnes)
{
  const char* const ipStr = "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff";
  EXPECT_EQ(convertIPv6Address(ipStr), ipStr);
}

TEST(AddrToStringTester, IPv6Realistic)
{
  const char* const ipStr = "2001:db8:85a3:0:0:8a2e:370:7334";
  EXPECT_EQ(convertIPv6Address(ipStr), ipStr);
}

TEST(AddrToStringTester, UnknownAddressFamily)
{
  sockaddr_storage addr = {};
  addr.ss_family = AF_UNIX;

  EXPECT_THROW(utils::addrToString(addr), std::runtime_error);
}

// Main function to run all tests
int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
