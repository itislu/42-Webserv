#include "ParsedServer.hpp"
#include "ConfigTypes.hpp"
#include "ParsedLocation.hpp"
#include <vector>

namespace config {

DirectiveMap& ParsedServer::getDirectives()
{
  return _directives;
}

const DirectiveMap& ParsedServer::getDirectives() const
{
  return _directives;
}

std::vector<ParsedLocation>& ParsedServer::getLocations()
{
  return _locations;
}

const std::vector<ParsedLocation>& ParsedServer::getLocations() const
{
  return _locations;
}

/*
  VALID KEYS

  listen
  server_name
  root
  index
  client_max_body_size
  error_page
  keepalive_timeout
  server
  allowed_methods

  error_log
*/

} // namespace config
