#include "config/DirectiveHandler.hpp"
#include "config/Config.hpp"
#include "config/LocationConfig.hpp"
#include "config/ServerConfig.hpp"

// ==================== Config ====================
template<>
const DirectiveHandler<Config>::Entry (&DirectiveHandler<Config>::_getEntries())[5] {
  static Entry entries[] = {{ "root", &DirectiveHandler<Config>::setRoot },
  { "keepalive_timeout", &DirectiveHandler<Config>::setTimeout },
  { "max_body_size", &DirectiveHandler<Config>::setMaxBodySize },
  { "error_page", &DirectiveHandler<Config>::setErrorPage },
  { 0, 0 }};
  return entries;
}

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
    { "allowed_methods", &DirectiveHandler<LocationConfig>::setAllowedMethods },
    { "autoindex", &DirectiveHandler<LocationConfig>::setAutoIndex },
    { "cgi_enabled", &DirectiveHandler<LocationConfig>::setCgi },
    { "cgi_pass", &DirectiveHandler<LocationConfig>::setCgiPass },
    { "cgi_extension", &DirectiveHandler<LocationConfig>::setCgiExtension },
    { "redirect", &DirectiveHandler<LocationConfig>::setRedirect },
    { "redirect_url", &DirectiveHandler<LocationConfig>::setRedirection },
    { "redirect_code", &DirectiveHandler<LocationConfig>::setRedirectCode },
    { 0, 0 }
  };

// =================================================
