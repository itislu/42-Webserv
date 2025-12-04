#include "Authority.hpp"

#include <sstream>
#include <string>

/* ************************************************************************** */
// PUBLIC

const std::string& Authority::getUserinfo() const
{
  return _userinfo;
}

const std::string& Authority::getHost() const
{
  return _host;
}

const std::string& Authority::getPort() const
{
  return _port;
}

void Authority::setUserinfo(const std::string& userinfo)
{
  _userinfo = userinfo;
}

void Authority::setHost(const std::string& host)
{
  _host = host;
}

void Authority::setPort(const std::string& port)
{
  _port = port;
}

std::string Authority::toString() const
{
  std::stringstream oss;

  oss << "{\n";
  oss << "  \"userinfo\": \"" << _userinfo << "\",\n";
  oss << "  \"host\": \"" << _host << "\",\n";
  oss << "  \"port\": \"" << _port << "\",\n";
  oss << "}";

  return oss.str();
}
