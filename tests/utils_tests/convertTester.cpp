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

TEST(AddrToStringTester, IPv4PrivateNetwork)
{
  const char* const ipStr = "192.168.1.100";
  EXPECT_EQ(convertIPv4Address(ipStr), ipStr);
}

TEST(AddrToStringTester, IPv4PublicDNS)
{
  const char* const ipStr = "8.8.8.8";
  EXPECT_EQ(convertIPv4Address(ipStr), ipStr);
}

TEST(AddrToStringTester, IPv4Multicast)
{
  const char* const ipStr = "224.0.0.1";
  EXPECT_EQ(convertIPv4Address(ipStr), ipStr);
}

TEST(AddrToStringTester, IPv4Broadcast)
{
  const char* const ipStr = "255.255.255.255";
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

TEST(AddrToStringTester, IPv6Documentation)
{
  const char* const ipStr = "2001:db8:85a3:0:0:8a2e:370:7334";
  EXPECT_EQ(convertIPv6Address(ipStr), ipStr);
}

TEST(AddrToStringTester, IPv6LinkLocal)
{
  const char* const ipStr = "fe80:0:0:0:1234:5678:90ab:cdef";
  EXPECT_EQ(convertIPv6Address(ipStr), ipStr);
}

TEST(AddrToStringTester, IPv6AllOnes)
{
  const char* const ipStr = "ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff";
  EXPECT_EQ(convertIPv6Address(ipStr), ipStr);
}

TEST(AddrToStringTester, IPv6GoogleDNS)
{
  const char* const ipStr = "2001:4860:4860:0:0:0:0:8888";
  EXPECT_EQ(convertIPv6Address(ipStr), ipStr);
}

TEST(AddrToStringTester, IPv6Mixed)
{
  const char* const ipStr = "2001:db8:0:0:0:0:0:1";
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
