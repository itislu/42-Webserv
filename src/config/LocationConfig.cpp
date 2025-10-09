#include "LocationConfig.hpp"
#include <cstddef>
#include <string>
#include <vector>

LocationConfig::LocationConfig()
  : _autoindex(false)
  , _maxBodysize()
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

void LocationConfig::getCgiExtension(const std::string& cgiExtension)
{
  _cgiExtension = cgiExtension;
}

void LocationConfig::isRedirect(bool redirect)
{
  _redirect = redirect;
}

void LocationConfig::getRedirection(const std::string& redirectUrl)
{
  _redirectUrl = redirectUrl;
}

void LocationConfig::getRedirectCode(int code)
{
  _redirectCode = code;
}
