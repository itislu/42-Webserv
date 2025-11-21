#include "Resource.hpp"
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
