#pragma once
#ifndef PARSE_VERSION_HPP
#define PARSE_VERSION_HPP

#include "http/TokenFinder.hpp"
#include <utils/IState.hpp>

class Client;
class ReadStartLine;

/* ************************************************************************** */
class ParseVersion : public IState<ReadStartLine>
{
public:
  explicit ParseVersion(ReadStartLine* context);
  ~ParseVersion();

  void run();

private:
  ParseVersion();
  ParseVersion(const ParseVersion& other);
  ParseVersion& operator=(const ParseVersion& other);

  Client* _client;
  TokenFinder _finder;
};

#endif // PARSE_URI_HPP
