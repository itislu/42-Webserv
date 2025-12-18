#pragma once
#ifndef HEADERS_HPP
#define HEADERS_HPP

#include <map>
#include <string>

/* ************************************************************************** */
class Headers
{
public:
  // the HeaderMap contains the header name in lowercase as key
  // and the HeaderPair contains the original header name + value
  struct HeaderPair
  {
    std::string name;
    std::string value;
  };
  typedef std::map<std::string, HeaderPair> HeaderMap;
  typedef HeaderMap::const_iterator const_iter;

  void setHeader(const std::string& key, const std::string& value);
  void addHeader(const std::string& key, const std::string& value);

  std::string toString() const;
  std::string toLogString() const;

  const std::string& at(const std::string& key) const;
  bool contains(const std::string& key) const;

  const_iter begin() const;
  const_iter end() const;

private:
  static void _formatInput(std::string& key, HeaderPair& headerPair);
  static void _addNew(HeaderPair& entry, const HeaderPair& headerPair);
  static void _setExisting(HeaderPair& entry, const HeaderPair& headerPair);
  static void _addExisting(HeaderPair& entry, const HeaderPair& headerPair);

  HeaderMap _headers;
};

#endif
