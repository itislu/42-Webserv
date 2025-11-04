
#include "ConfigBuilder.hpp"
#include "config/Config.hpp"
#include "config/Converters.hpp"
#include "config/ParsedConfig.hpp"
#include "config/ParsedServer.hpp"
#include "config/ServerBuilder.hpp"
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

const ConfigBuilder::DirectiveHandler ConfigBuilder::_handlers[] = {
  { "root", ConfigBuilder::setRoot },
  { "keepalive_timeout", ConfigBuilder::setTimeout },
  { "max_body_size", ConfigBuilder::setMaxBodySize },
};

const std::size_t ConfigBuilder::_handlersSize =
  sizeof(ConfigBuilder::_handlers) / sizeof(ConfigBuilder::_handlers[0]);

void ConfigBuilder::setRoot(const std::vector<std::string>& value,
                            Config& config)
{
  if (value.empty() || value.size() > 1) {
    throw std::invalid_argument(
      "config: invalid number of arguments in root directive");
  }
  config.setRoot(value[0]);
}
void ConfigBuilder::setMaxBodySize(const std::vector<std::string>& value,
                                   Config& config)
{
  if (value.empty() || value.size() > 1) {
    throw std::invalid_argument(
      "config: invalid number of arguments in max_body_size directive");
  }
  try {
    const std::size_t bytes = toSizeT(value[0]);
    config.setMaxBodySize(bytes);
  } catch (const std::exception& e) {
    throw std::invalid_argument(
      std::string("config: invalid argument for max_body_size, because ") +
      e.what());
  }
}
void ConfigBuilder::setTimeout(const std::vector<std::string>& value,
                               Config& config)
{
  if (value.empty() || value.size() > 1) {
    throw std::invalid_argument(
      "config: invalid number of arguments in keepalive_timeout directive");
  }
  try {
    const std::size_t timeout = toSizeT(value[0]);
    config.setTimeout(timeout);
  } catch (const std::exception& e) {
    throw std::invalid_argument(
      std::string("config: invalid argument for max_body_size, because ") +
      e.what());
  }
}

void ConfigBuilder::checkDirectiveHandler(
  const std::string& key,
  const std::vector<std::string>& values,
  Config& config)
{
  for (std::size_t i = 0; i < _handlersSize; ++i) {
    // std::cout << "Compare: [" << key << "] : [" << _handlers[i].key << "]\n";
    //  NOLINTBEGIN (cppcoreguidelines-pro-bounds-constant-array-index)
    if (std::strcmp(key.c_str(), _handlers[i].key) == 0) {
      std::cout << "VALID IDENTIFIER!\n";
      _handlers[i].func(values, config);
      return;
    }
    // NOLINTEND
  }
  // TODO: throw because not a valid identifier
  std::cout << "NO VALID IDENTIFIER!\n";
}

void ConfigBuilder::buildServers(const std::vector<ParsedServer>& servers,
                                 Config& config)
{
  if (servers.empty()) {
    throw std::runtime_error("No server");
  }
  for (std::vector<ParsedServer>::const_iterator it = servers.begin();
       it != servers.end();
       ++it) {
    config.addServer(ServerBuilder::build(*it, config));
  }
}

void ConfigBuilder::buildDirectives(const ParsedConfig::Directive& directives,
                                    Config& config)
{
  for (ParsedConfig::Directive::const_iterator it = directives.begin();
       it != directives.end();
       ++it) {
    const std::string& key = it->first;
    const std::vector<std::string>& values = it->second;
    checkDirectiveHandler(key, values, config);
  }
}

Config ConfigBuilder::build(const ParsedConfig& parsed)
{
  Config config;

  buildDirectives(parsed.getDirective(), config);
  // buildServers(parsed.getServers(), config);
  config.setDefaultTimeout();

  std::cout << config;

  return config;
}

/*
  - no duplicates in global directive allowed
  - every directive must have only one value
  - must have directives:
    - timeout
    - maxbodysize
    -
  - at least 1 server

*/
