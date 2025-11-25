#include "Resource.hpp"
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

void Resource::setType(Type type)
{
  _type = type;
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
