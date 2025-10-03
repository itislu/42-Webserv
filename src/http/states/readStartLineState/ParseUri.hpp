#pragma once
#ifndef PARSE_URI_HPP
#define PARSE_URI_HPP

#include "http/states/readStartLineState/IReadStartLineState.hpp"

class Client;
class ReadStartLine;

/* ************************************************************************** */
class ParseUri : public IReadStartLineState
{
public:
  explicit ParseUri(ReadStartLine* state);
  ~ParseUri();

  void run();

private:
  ParseUri();
  ParseUri(const ParseUri& other);
  ParseUri& operator=(const ParseUri& other);

  Client* _client;
};

#endif // PARSE_URI_HPP
