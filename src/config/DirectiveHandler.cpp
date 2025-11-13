#include "config/DirectiveHandler.hpp"
#include "config/Config.hpp"
#include "config/Converters.hpp"
#include "config/LocationConfig.hpp"
#include "config/ServerConfig.hpp"
#include <cstddef>
#include <exception>
#include <stdexcept>
#include <string>
#include <vector>

// ==================== Config ====================

Entries<Config>::Entry Entries<Config>::entries[] = {
  { "root", setRoot },
  { "keepalive_timeout", setTimeout },
  { "max_body_size", setMaxBodySize },
  { "error_page", setErrorPage },
  { 0, 0 }
};

// ==================== ServerConfig ====================

Entries<ServerConfig>::Entry Entries<ServerConfig>::entries[] = {
  { "listen", setPorts },
  { "server_name", setHostnames },
  { "root", setRoot },
  { "index", setIndex },
  { "keepalive_timeout", setTimeout },
  { "max_body_size", setMaxBodySize },
  { "error_page", setErrorPage },
  { "allowed_methods", setAllowedMethods },
  { 0, 0 }
};

// ==================== LocationConfig ====================

Entries<LocationConfig>::Entry Entries<LocationConfig>::entries[] = {
  { "root", setRoot },
  { "index", setIndex },
  { "max_body_size", setMaxBodySize },
  { "error_page", setErrorPage },
  { "allowed_methods", setAllowedMethods },
  { "autoindex", setAutoIndex },
  { "cgi_enabled", setCgi },
  { "cgi_pass", setCgiPass },
  { "cgi_extension", setCgiExtension },
  { "redirect", setRedirect },
  { "redirect_url", setRedirection },
  { "redirect_code", setRedirectCode },
  { 0, 0 }
};

// =================================================

// ============== Config - Setters ====================
void Entries<Config>::setTimeout(const std::vector<std::string>& values,
                                 Config& config)
{
  if (values.size() != 1) {
    throw std::invalid_argument(
      "keepalive_timeout: invalid number of arguments");
  }
  long timeout = 0;
  try {
    timeout = toLong(values[0]);
  } catch (const std::exception& e) {
    throw std::invalid_argument(
      std::string("keepalive_timeout: invalid argument: ") + e.what());
  }
  config.setTimeout(timeout);
}

// ============== Server - Setters ====================
void Entries<ServerConfig>::setTimeout(const std::vector<std::string>& values,
                                       ServerConfig& config)
{
  if (values.size() != 1) {
    throw std::invalid_argument(
      "keepalive_timeout: invalid number of arguments");
  }
  long timeout = 0;
  try {
    timeout = toLong(values[0]);
  } catch (const std::exception& e) {
    throw std::invalid_argument(
      std::string("keepalive_timeout: invalid argument: ") + e.what());
  }
  config.setTimeout(timeout);
}

void Entries<ServerConfig>::setIndex(const std::vector<std::string>& values,
                                     ServerConfig& config)
{
  if (values.size() != 1) {
    throw std::invalid_argument("index: invalid number of arguments");
  }
  config.setIndex(values[0]);
}

void Entries<ServerConfig>::setAllowedMethods(
  const std::vector<std::string>& values,
  ServerConfig& config)
{
  if (values.empty()) {
    throw std::invalid_argument("allowed_methods: invalid number of arguments");
  }

  for (std::size_t i = 0; i < values.size(); ++i) {
    if (!isMethod(values[i])) {
      throw std::invalid_argument("allowed_methods: invalid method: " +
                                  values[i]);
    }
    config.setAllowedMethod(values[i]);
  }
}

void Entries<ServerConfig>::setPorts(const std::vector<std::string>& values,
                                     ServerConfig& config)
{
  for (std::size_t i = 0; i < values.size(); ++i) {
    int port = 0;
    try {
      port = toPort(values[i]);
      config.addPort(port);
    } catch (const std::exception& e) {
      throw std::invalid_argument(std::string("listen: invalid argument: ") +
                                  e.what());
    }
  }
}

void Entries<ServerConfig>::setHostnames(const std::vector<std::string>& values,
                                         ServerConfig& config)
{
  for (std::size_t i = 0; i < values.size(); ++i) {
    config.addHostName(values[i]);
  }
}

// ============== Location - Setters ====================

void Entries<LocationConfig>::setIndex(const std::vector<std::string>& values,
                                       LocationConfig& config)
{
  if (values.size() != 1) {
    throw std::invalid_argument("index: invalid number of arguments");
  }
  config.setIndex(values[0]);
}

void Entries<LocationConfig>::setAllowedMethods(
  const std::vector<std::string>& values,
  LocationConfig& config)
{
  if (values.empty()) {
    throw std::invalid_argument("allowed_methods: invalid number of arguments");
  }

  for (std::size_t i = 0; i < values.size(); ++i) {
    if (!isMethod(values[i])) {
      throw std::invalid_argument("allowed_methods: invalid method: " +
                                  values[i]);
    }
    config.setAllowedMethod(values[i]);
  }
}

void Entries<LocationConfig>::setAutoIndex(
  const std::vector<std::string>& values,
  LocationConfig& config)
{
  if (values.size() != 1) {
    throw std::invalid_argument("autoindex: invalid number of arguments");
  }

  bool enabled = false;
  try {
    enabled = toBool(values[0]);
  } catch (const std::exception& e) {
    throw std::invalid_argument(std::string("autoindex: invalid argument: ") +
                                e.what());
  }
  config.setAutoIndex(enabled);
}

void Entries<LocationConfig>::setCgi(const std::vector<std::string>& values,
                                     LocationConfig& config)
{
  if (values.size() != 1) {
    throw std::invalid_argument("autoindex: invalid number of arguments");
  }

  bool enabled = false;
  try {
    enabled = toBool(values[0]);
  } catch (const std::exception& e) {
    throw std::invalid_argument(std::string("autoindex: invalid argument: ") +
                                e.what());
  }
  config.setCgi(enabled);
}

void Entries<LocationConfig>::setCgiPass(const std::vector<std::string>& values,
                                         LocationConfig& config)
{
  if (values.size() != 1) {
    throw std::invalid_argument("cgi_pass: invalid number of arguments");
  }
  config.setCgiPass(values[0]);
}

void Entries<LocationConfig>::setCgiExtension(
  const std::vector<std::string>& values,
  LocationConfig& config)
{
  if (values.size() != 1) {
    throw std::invalid_argument("cgi_extension: invalid number of arguments");
  }
  config.setCgiExtension(values[0]);
}

void Entries<LocationConfig>::setRedirect(
  const std::vector<std::string>& values,
  LocationConfig& config)
{
  if (values.size() != 1) {
    throw std::invalid_argument("redirect: invalid number of arguments");
  }
  bool enabled = false;
  try {
    enabled = toBool(values[0]);
  } catch (const std::exception& e) {
    throw std::invalid_argument(std::string("redirect: invalid argument: ") +
                                e.what());
  }
  config.setRedirect(enabled);
}

void Entries<LocationConfig>::setRedirection(
  const std::vector<std::string>& values,
  LocationConfig& config)
{
  if (values.size() != 1) {
    throw std::invalid_argument("redirection: invalid number of arguments");
  }
  config.setRedirection(values[0]);
}

void Entries<LocationConfig>::setRedirectCode(
  const std::vector<std::string>& values,
  LocationConfig& config)
{
  if (values.size() != 1) {
    throw std::invalid_argument("redirect_code: invalid number of arguments");
  }
  int code = 0;
  try {
    code = toCode(values[0]);
  } catch (const std::exception& e) {
    throw std::invalid_argument(
      std::string("redirect_code: invalid argument: ") + e.what());
  }
  config.setRedirectCode(code);
}
