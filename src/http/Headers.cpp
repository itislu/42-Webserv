#include "Headers.hpp"

#include <http/http.hpp>
#include <libftpp/string.hpp>

#include <string>

/* ************************************************************************** */
// PUBLIC

void Headers::setHeader(const std::string& key, const std::string& value)
{
  std::string keyFormated = key;
  HeaderPair headerPair = { key, value };
  _formatInput(keyFormated, headerPair);

  if (keyFormated.empty()) {
    return;
  }

  HeaderPair& entry = _headers[keyFormated];
  if (entry.name.empty()) {
    _addNew(entry, headerPair);
  } else {
    _setExisting(entry, headerPair);
  }
}

void Headers::addHeader(const std::string& key, const std::string& value)
{
  std::string keyFormated = key;
  HeaderPair headerPair = { key, value };
  _formatInput(keyFormated, headerPair);

  if (keyFormated.empty()) {
    return;
  }

  HeaderPair& entry = _headers[keyFormated];
  if (entry.name.empty()) {
    _addNew(entry, headerPair);
  } else {
    _addExisting(entry, headerPair);
  }
}

std::string Headers::toString() const
{
  std::string oss;
  for (HeaderMap::const_iterator iter = _headers.begin();
       iter != _headers.end();
       ++iter) {
    const HeaderPair& header = iter->second;
    oss.append(header.name).append(": ");
    oss.append(header.value).append(http::CRLF);
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
    oss.append(header.name).append(": ");
    oss.append(header.value);
    oss.append("\"\n");
  }
  return oss;
}

const std::string& Headers::at(const std::string& key) const
{
  const HeaderPair& header = _headers.at(ft::to_lower(key));
  return header.value;
}

bool Headers::contains(const std::string& key) const
{
  return _headers.find(ft::to_lower(key)) != _headers.end();
}

Headers::const_iter Headers::begin() const
{
  return _headers.begin();
}
Headers::const_iter Headers::end() const
{
  return _headers.end();
}

/* ************************************************************************** */
// PRIVATE

void Headers::_formatInput(std::string& key, HeaderPair& headerPair)
{
  ft::trim(key);
  ft::trim(headerPair.name);
  ft::trim(headerPair.value);

  ft::to_lower(key);
}

void Headers::_addNew(HeaderPair& entry, const HeaderPair& headerPair)
{
  entry = headerPair;
}

void Headers::_setExisting(HeaderPair& entry, const HeaderPair& headerPair)
{
  entry.value = headerPair.value;
}

void Headers::_addExisting(HeaderPair& entry, const HeaderPair& headerPair)
{
  entry.value.append(", ").append(headerPair.value);
}
