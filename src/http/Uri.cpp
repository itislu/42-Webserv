#include "Uri.hpp"

#include <sstream>
#include <string>

/* ************************************************************************** */
// PUBLIC

const std::string& Uri::getScheme() const
{
  return _scheme;
}

void Uri::setScheme(const std::string& str)
{
  _scheme = str;
}

const std::string& Uri::getAuthority() const
{
  return _authority;
}

void Uri::setAuthority(const std::string& str)
{
  _authority = str;
}

const std::string& Uri::getPath() const
{
  return _path;
}

void Uri::setPath(const std::string& str)
{
  _path = str;
}

const std::string& Uri::getQuery() const
{
  return _query;
}

void Uri::setQuery(const std::string& str)
{
  _query = str;
}

const std::string& Uri::getFragment() const
{
  return _fragment;
}

void Uri::setFragment(const std::string& str)
{
  _fragment = str;
}

std::string Uri::toString() const
{
  std::stringstream oss;

  oss << "{\n";
  oss << "  \"scheme\": \"" << _scheme << "\",\n";
  oss << "  \"authority\": \"" << _authority << "\",\n";
  oss << "  \"path\": \"" << _path << "\",\n";
  oss << "  \"query\": \"" << _query << "\",\n";
  oss << "  \"fragment\": \"" << _fragment << "\"\n";
  oss << "}";

  return oss.str();
}

/* ************************************************************************** */
// PRIVATE
