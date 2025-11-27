#include "Resource.hpp"
#include "config/LocationConfig.hpp"
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

const config::LocationConfig* Resource::getLocation()
{
  return _location;
}

const config::ServerConfig* Resource::getServer()
{
  return _server;
}

void Resource::setType(Type type)
{
  _type = type;
}

void Resource::setLocation(const config::LocationConfig* location)
{
  _location = location;
}

void Resource::setServer(const config::ServerConfig* server)
{
  _server = server;
}

std::string Resource::_typeToString()
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
}

std::string Resource::toString()
{
  std::stringstream oss;
  oss << " Resource:\n";
  oss << "  path: " << _path << "\n";
  oss << "  type: " << _typeToString() << "\n";

  return oss.str();
}
