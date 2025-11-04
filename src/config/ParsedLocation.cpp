#include "ParsedLocation.hpp"
#include <map>
#include <string>
#include <vector>

ParsedLocation::ParsedLocation(const std::string& path)
  : _path(path)
{
}

void ParsedLocation::setPath(const std::string& path)
{
  _path = path;
}

std::map<std::string, std::vector<std::string> >& ParsedLocation::getDirective()
{
  return _directives;
}

const std::map<std::string, std::vector<std::string> >&
ParsedLocation::getDirective() const
{
  return _directives;
}

const std::string& ParsedLocation::getPath() const
{
  return _path;
}

/*
  VALID KEYS

  root
  autoindex
  index
  client_max_body_size
  error_page
  keepalive_timeout
  allowed_methods

  cgi_enabled
  cgi_pass
  cgi_extension

  redirect
  redirectUrl
  redirectCode
*/
