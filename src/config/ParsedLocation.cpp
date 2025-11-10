#include "ParsedLocation.hpp"
#include "config/ConfigTypes.hpp"
#include <string>

ParsedLocation::ParsedLocation(const std::string& path)
  : _path(path)
{
}

void ParsedLocation::setPath(const std::string& path)
{
  _path = path;
}

DirectiveMap& ParsedLocation::getDirective()
{
  return _directives;
}

const DirectiveMap& ParsedLocation::getDirective() const
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
