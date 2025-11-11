#include "Headers.hpp"

#include <http/http.hpp>
#include <libftpp/string.hpp>

#include <string>

/* ************************************************************************** */
// PUBLIC

void Headers::addHeader(const std::string& key, const std::string& value)
{
  std::string keyFormated = key;
  std::string valueFormated = value;
  _formatInput(keyFormated, valueFormated);

  if (keyFormated.empty()) {
    return;
  }

  const HeaderMap::const_iterator iter = _headers.find(keyFormated);
  if (iter == _headers.end()) {
    _addNew(keyFormated, valueFormated);
  } else {
    _addExisting(keyFormated, valueFormated);
  }
}

std::string Headers::toString() const
{
  std::string oss;
  for (HeaderMap::const_iterator iter = _headers.begin();
       iter != _headers.end();
       ++iter) {
    oss.append(iter->first + ": ");
    oss.append(iter->second + http::CRLF);
  }
  return oss;
}

std::string Headers::toLogString() const
{
  std::string oss;
  for (HeaderMap::const_iterator iter = _headers.begin();
       iter != _headers.end();
       ++iter) {
    oss.append("  \"");
    oss.append(iter->first + ": ");
    oss.append(iter->second);
    oss.append("\"\n");
  }
  return oss;
}

const std::string& Headers::operator[](const std::string& key) const
{
  return _headers.at(ft::to_lower(key));
}

bool Headers::contains(const std::string& key) const
{
  return _headers.find(ft::to_lower(key)) != _headers.end();
}

/* ************************************************************************** */
// PRIVATE

void Headers::_formatInput(std::string& key, std::string& value)
{
  ft::trim(key);
  ft::trim(value);

  ft::to_lower(key);
}

void Headers::_addNew(const std::string& key, const std::string& value)
{
  _headers[key] = value;
}

void Headers::_addExisting(const std::string& key, const std::string& value)
{
  _headers[key].append(", " + value);
}
