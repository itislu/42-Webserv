#include "ParsedLocation.hpp"
#include "ConfigTypes.hpp"
#include <string>

namespace config {

ParsedLocation::ParsedLocation(const std::string& path)
  : _path(path)
{
}

void ParsedLocation::setPath(const std::string& path)
{
  _path = path;
}

DirectiveMap& ParsedLocation::getDirectives()
{
  return _directives;
}

const DirectiveMap& ParsedLocation::getDirectives() const
{
  return _directives;
}

const std::string& ParsedLocation::getPath() const
{
  return _path;
}

} // namespace config

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
