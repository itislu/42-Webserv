#include "Headers.hpp"

#include <http/http.hpp>
#include <libftpp/string.hpp>

#include <string>

/* ************************************************************************** */
// PUBLIC

void Headers::setHeader(const std::string& key, const std::string& value)
{
  std::string keyFormated = key;
  HeaderPair headerPair(key, value);
  _formatInput(keyFormated, headerPair);

  if (keyFormated.empty()) {
    return;
  }

  const HeaderMap::const_iterator iter = _headers.find(keyFormated);
  if (iter == _headers.end()) {
    _addNew(keyFormated, headerPair);
  } else {
    _setExisting(keyFormated, headerPair);
  }
}

void Headers::addHeader(const std::string& key, const std::string& value)
{
  std::string keyFormated = key;
  HeaderPair headerPair(key, value);
  _formatInput(keyFormated, headerPair);

  if (keyFormated.empty()) {
    return;
  }

  const HeaderMap::const_iterator iter = _headers.find(keyFormated);
  if (iter == _headers.end()) {
    _addNew(keyFormated, headerPair);
  } else {
    _addExisting(keyFormated, headerPair);
  }
}

std::string Headers::toString() const
{
  std::string oss;
  for (HeaderMap::const_iterator iter = _headers.begin();
       iter != _headers.end();
       ++iter) {
    const HeaderPair& header = iter->second;
    oss.append(header.first).append(": ");
    oss.append(header.second).append(http::CRLF);
  }
  return oss;
}

std::string Headers::toLogString() const
{
  std::string oss;
  for (HeaderMap::const_iterator iter = _headers.begin();
       iter != _headers.end();
       ++iter) {
    const HeaderPair& header = iter->second;
    oss.append("  \"");
    oss.append(header.first).append(": ");
    oss.append(header.second);
    oss.append("\"\n");
  }
  return oss;
}

const std::string& Headers::at(const std::string& key) const
{
  const HeaderPair& header = _headers.at(ft::to_lower(key));
  return header.second;
}

bool Headers::contains(const std::string& key) const
{
  return _headers.find(ft::to_lower(key)) != _headers.end();
}

/* ************************************************************************** */
// PRIVATE

void Headers::_formatInput(std::string& key, HeaderPair& headerPair)
{
  ft::trim(key);
  ft::trim(headerPair.first);
  ft::trim(headerPair.second);

  ft::to_lower(key);
}

void Headers::_addNew(const std::string& key, const HeaderPair& headerPair)
{
  _headers[key] = headerPair;
}

void Headers::_setExisting(const std::string& key, const HeaderPair& headerPair)
{
  HeaderPair& header = _headers[key];
  header.second = headerPair.second;
}

void Headers::_addExisting(const std::string& key, const HeaderPair& headerPair)
{
  HeaderPair& header = _headers[key];
  header.second.append(", ").append(headerPair.second);
}
