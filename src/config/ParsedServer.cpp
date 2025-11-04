#include "ParsedServer.hpp"
#include "config/ParsedLocation.hpp"
#include <map>
#include <string>
#include <vector>

std::map<std::string, std::vector<std::string> >& ParsedServer::getDirective()
{
  return _directives;
}

const std::map<std::string, std::vector<std::string> >&
ParsedServer::getDirective() const
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
