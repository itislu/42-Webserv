#include "Resource.hpp"
#include "config/LocationConfig.hpp"
#include "libftpp/utility.hpp"
#include <sstream>
#include <string>

const std::string& Resource::getPath() const
{
  return _path;
}

void Resource::setPath(const std::string& path)
{
  _path = path;
}

Resource::Type Resource::getType() const
{
  return _type;
}

const LocationConfig* Resource::getLocation() const
{
  return _location;
}

const ServerConfig* Resource::getServer() const
{
  return _server;
}

void Resource::setType(Type type)
{
  _type = type;
}

void Resource::setLocation(const LocationConfig* location)
{
  _location = location;
}

void Resource::setServer(const ServerConfig* server)
{
  _server = server;
}

std::string Resource::_typeToString() const
{
  switch (_type) {
    case File:
      return "File";
    case Autoindex:
      return "Autoindex";
    case Cgi:
      return "Cgi";
    case Error:
      return "Error";
  }
  FT_UNREACHABLE();
}

std::string Resource::toString() const
{
  std::ostringstream oss;
  oss << " Resource:\n";
  oss << "  path: " << _path << "\n";
  oss << "  type: " << _typeToString() << "\n";

  return oss.str();
}
