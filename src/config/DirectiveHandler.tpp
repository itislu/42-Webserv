#include "config/Config.hpp"
#include "config/DirectiveHandler.hpp"
#include "config/LocationConfig.hpp"
#include "config/ParsedConfig.hpp"
#include "config/ServerConfig.hpp"
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

// ==================== Config ====================
template<>
const DirectiveHandler<Config>::Entry DirectiveHandler<Config>::_entries[] = {
  { "root", &DirectiveHandler<Config>::setRoot },
  { "keepalive_timeout", &DirectiveHandler<Config>::setTimeout },
  { 0, 0 }
};

// ==================== ServerConfig ====================
template<>
const DirectiveHandler<ServerConfig>::Entry
  DirectiveHandler<ServerConfig>::_entries[] = {
    { "listen", &DirectiveHandler<ServerConfig>::setPorts },
    { "server_name", &DirectiveHandler<ServerConfig>::setHostnames },
    { "root", &DirectiveHandler<ServerConfig>::setRoot },
    { "index", &DirectiveHandler<ServerConfig>::setIndex },
    { "error_page", &DirectiveHandler<ServerConfig>::setErrorPage },
    { "max_body_size", &DirectiveHandler<ServerConfig>::setMaxBodySize },
    { "allowed_methods", &DirectiveHandler<ServerConfig>::setAllowedMethods },
    { "keepalive_timeout", &DirectiveHandler<ServerConfig>::setTimeout },
    { 0, 0 }
  };

// ==================== LocationConfig ====================
template<>
const DirectiveHandler<LocationConfig>::Entry
  DirectiveHandler<LocationConfig>::_entries[] = {
    { "root", &DirectiveHandler<LocationConfig>::setRoot },
    { "index", &DirectiveHandler<LocationConfig>::setIndex },
    { "error_page", &DirectiveHandler<LocationConfig>::setErrorPage },
    { "max_body_size", &DirectiveHandler<LocationConfig>::setMaxBodySize },
    { "allowed_methods", &DirectiveHandler<LocationConfig>::setAllowedMethods },
    { "autoindex", &DirectiveHandler<LocationConfig>::setTimeout },
    { "cgi_enabled", &DirectiveHandler<LocationConfig>::setTimeout },
    { "cgi_pass", &DirectiveHandler<LocationConfig>::setTimeout },
    { "cgi_extension", &DirectiveHandler<LocationConfig>::setTimeout },
    { "redirect", &DirectiveHandler<LocationConfig>::setTimeout },
    { "redirect_url", &DirectiveHandler<LocationConfig>::setTimeout },
    { "redirect_code", &DirectiveHandler<LocationConfig>::setTimeout },
    { 0, 0 }
  };

// =================================================

template<typename ConfigType>
void DirectiveHandler<ConfigType>::checkDirectiveHandler(
  const std::string& key,
  const std::vector<std::string>& values,
  ConfigType& config)
{
  for (std::size_t i = 0; _entries[i].key != 0; ++i) {
    if (std::strcmp(_entries[i].key, key.c_str()) == 0) {
      _entries[i].func(values, config);
      return;
    }
  }
  throw std::invalid_argument("Invalid directive: " + key);
}

template<typename ConfigType>
void DirectiveHandler<ConfigType>::buildDirectives(
  const ParsedConfig::Directive& directives,
  ConfigType& config)
{
  for (typename ConfigType::Directive::const_iterator it = directives.begin();
       it != directives.end();
       ++it) {
    const std::string& key = it->first;
    const std::vector<std::string>& values = it->second;
    checkDirectiveHandler(key, values, config);
  }
}

// ==================== Setters ====================
template<typename ConfigType>
void DirectiveHandler<ConfigType>::setRoot(
  const std::vector<std::string>& values,
  ConfigType& config)
{
  if (values.size() != 1) {
    throw std::invalid_argument("root: invalid number of arguments");
  }
  config.setRoot(values[0]);
}

template<typename ConfigType>
void DirectiveHandler<ConfigType>::setIndex(
  const std::vector<std::string>& values,
  ConfigType& config)
{
  if (values.size() != 1) {
    throw std::invalid_argument("index: invalid number of arguments");
  }
  config.setIndex(values[0]);
}

template<typename ConfigType>
void DirectiveHandler<ConfigType>::setTimeout(
  const std::vector<std::string>& values,
  ConfigType& config)
{
  if (values.size() != 1) {
    throw std::invalid_argument(
      "keepalive_timeout: invalid number of arguments");
  }
  std::size_t timeout = std::strtoul(values[0].c_str(), 0, 10);
  config.setTimeout(timeout);
}

template<typename ConfigType>
void DirectiveHandler<ConfigType>::setMaxBodySize(
  const std::vector<std::string>& values,
  ConfigType& config)
{
  if (values.size() != 1)
    throw std::invalid_argument("max_body_size: invalid number of arguments");
  std::size_t size = std::strtoul(values[0].c_str(), 0, 10);
  config.setMaxBodySize(size);
}

template<typename ConfigType>
void DirectiveHandler<ConfigType>::setErrorPage(
  const std::vector<std::string>& values,
  ConfigType& config)
{
  if (values.size() != 2)
    throw std::invalid_argument("error_page: invalid number of arguments");
  int code = std::atoi(values[0].c_str());
  config.addErrorPage(code, values[1]);
}

template<typename ConfigType>
void DirectiveHandler<ConfigType>::setPorts(
  const std::vector<std::string>& values,
  ConfigType& config)
{
  for (std::size_t i = 0; i < values.size(); ++i) {
    config.addPort(std::strtol(values[i].c_str()));
  }
}

template<typename ConfigType>
void DirectiveHandler<ConfigType>::setHostnames(
  const std::vector<std::string>& values,
  ConfigType& config)
{
  for (std::size_t i = 0; i < values.size(); ++i) {
    config.addHostName(values[i]);
  }
}

template<typename ConfigType>
void DirectiveHandler<ConfigType>::setAllowedMethods(
  const std::vector<std::string>& values,
  ConfigType& config)
{
  for (std::size_t i = 0; i < values.size(); ++i) {
    config.addAllowedMethod(values[i]);
  }
}
