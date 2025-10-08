#pragma once
#ifndef PARSE_METHOD_HPP
#define PARSE_METHOD_HPP

#include "http/TokenFinder.hpp"
#include "utils/IState.hpp"

class Client;
class ReadStartLine;

/* ************************************************************************** */
class ParseMethod : public IState<ReadStartLine>
{
public:
  explicit ParseMethod(ReadStartLine* context);
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
