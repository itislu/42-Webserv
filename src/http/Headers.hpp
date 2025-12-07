#pragma once
#ifndef HEADERS_HPP
#define HEADERS_HPP

#include <map>
#include <string>
#include <utility>

/* ************************************************************************** */
class Headers
{
public:
  void setHeader(const std::string& key, const std::string& value);
  void addHeader(const std::string& key, const std::string& value);

  std::string toString() const;
  std::string toLogString() const;

  const std::string& at(const std::string& key) const;
  bool contains(const std::string& key) const;

private:
  // the HeaderMap contains the header name in lowercase as key
  // and the HeaderPair contains the original header name + value
  typedef std::pair<std::string, std::string> HeaderPair;
  typedef std::map<std::string, HeaderPair> HeaderMap;

  static void _formatInput(std::string& key, HeaderPair& headerPair);

  HeaderMap _headers;
};

#endif
