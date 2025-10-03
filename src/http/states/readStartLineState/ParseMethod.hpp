#pragma once
#ifndef PARSE_METHOD_HPP
#define PARSE_METHOD_HPP

#include "http/TokenFinder.hpp"
#include <http/states/readStartLineState/IReadStartLineState.hpp>

class Client;
class ReadStartLine;

/* ************************************************************************** */
class ParseMethod : public IReadStartLineState
{
public:
  explicit ParseMethod(ReadStartLine* state);
  ~ParseMethod();

  void run();

private:
  ParseMethod();
  ParseMethod(const ParseMethod& other);
  ParseMethod& operator=(const ParseMethod& other);

  Client* _client;
  TokenFinder _finder;
};

#endif // PARSE_METHOD_HPP
