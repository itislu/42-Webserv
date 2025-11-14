#include "LocationConfig.hpp"
#include "config/ServerConfig.hpp"
#include <cstddef>
#include <map>
#include <set>
#include <string>
#include <vector>

LocationConfig::LocationConfig(const ServerConfig& serverConfig)
  : _root(serverConfig.getRoot())
  , _autoindex(false)
  , _allowedMethods(serverConfig.getAllowedMethods())
  , _index(serverConfig.getIndex())
  , _maxBodysize(serverConfig.getMaxBodySize())
  , _cgiEnabled(false)
  , _redirect(false)
  , _redirectCode()
{
}

// GETTERS
const std::string& LocationConfig::getPath() const
{
  return _path;
}

const std::string& LocationConfig::getRoot() const
{
  return _root;
}

bool LocationConfig::isAutoIndex() const
{
  return _autoindex;
}

const std::set<std::string>& LocationConfig::getAllowedMethods() const
{
  return _allowedMethods;
}

const std::string& LocationConfig::getIndex() const
{
  return _index;
}

std::size_t LocationConfig::getMaxBodySize() const
{
  return _maxBodysize;
}

bool LocationConfig::isCgi() const
{
  return _cgiEnabled;
}

const std::string& LocationConfig::getCgiPass() const
{
  return _cgiPass;
}

const std::string& LocationConfig::getCgiExtension() const
{
  return _cgiExtension;
}

bool LocationConfig::isRedirect() const
{
  return _redirect;
}

const std::string& LocationConfig::getRedirection() const
{
  return _redirectUrl;
}

int LocationConfig::getRedirectCode() const
{
  return _redirectCode;
}

const std::map<int, std::string>& LocationConfig::getErrorPages() const
{
  return _errorPages;
}

/* returns an empty string at the moment if not found */
const std::string& LocationConfig::getErrorPage(int code) const
{
  const std::map<int, std::string>::const_iterator iter =
    _errorPages.find(code);
  if (iter != _errorPages.end()) {
    return iter->second;
  }
  static const std::string empty;
  return empty;
}

// SETTERS
void LocationConfig::setPath(const std::string& path)
{
  _path = path;
}

void LocationConfig::setRoot(const std::string& root)
{
  _root = root;
}

void LocationConfig::setAutoIndex(bool autoindex)
{
  _autoindex = autoindex;
}

void LocationConfig::addAllowedMethod(const std::string& method)
{
  _allowedMethods.insert(method);
}

void LocationConfig::setErrorPages(const std::vector<int>& codes,
                                   const std::string& path)
{
  for (std::size_t i = 0; i < codes.size(); ++i) {
    addErrorPage(codes[i], path);
  }
}

void LocationConfig::addErrorPage(int code, const std::string& path)
{
  _errorPages[code] = path;
}

void LocationConfig::setIndex(const std::string& index)
{
  _index = index;
}

void LocationConfig::setMaxBodySize(std::size_t size)
{
  _maxBodysize = size;
}

void LocationConfig::setCgi(bool cgi)
{
  _cgiEnabled = cgi;
}

void LocationConfig::setCgiPass(const std::string& cgiPass)
{
  _cgiPass = cgiPass;
}

void LocationConfig::setCgiExtension(const std::string& cgiExtension)
{
  _cgiExtension = cgiExtension;
}

void LocationConfig::setRedirect(bool redirect)
{
  _redirect = redirect;
}

void LocationConfig::setRedirection(const std::string& redirectUrl)
{
  _redirectUrl = redirectUrl;
}

void LocationConfig::setRedirectCode(int code)
{
  _redirectCode = code;
}
