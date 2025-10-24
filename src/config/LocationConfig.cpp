#include "LocationConfig.hpp"
#include "config/ServerConfig.hpp"
#include <cstddef>
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

bool LocationConfig::isAutoindex() const
{
  return _autoindex;
}

const std::vector<std::string>& LocationConfig::getAllowedMethods() const
{
  return _allowedMethods;
}

const std::string& LocationConfig::getIndex() const
{
  return _index;
}

size_t LocationConfig::getMaxSize() const
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

// SETTERS
void LocationConfig::setPath(const std::string& path)
{
  _path = path;
}

void LocationConfig::setRoot(const std::string& root)
{
  _root = root;
}

void LocationConfig::setAutoindex(bool autoindex)
{
  _autoindex = autoindex;
}

void LocationConfig::setAllowedMethods(const std::string& method)
{
  _allowedMethods.clear();
  _allowedMethods.push_back(method);
}

void LocationConfig::addAllowedMethods(const std::string& method)
{
  _allowedMethods.push_back(method);
}

void LocationConfig::setIndex(const std::string& index)
{
  _index = index;
}

void LocationConfig::setMaxSize(std::size_t size)
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
