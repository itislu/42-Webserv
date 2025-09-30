#pragma once
#include "client/Client.hpp"
#ifndef STATE_READ_START_LINE_HPP
#define STATE_READ_START_LINE_HPP

#include "http/states/IState.hpp"
#include <cstddef>

class Client;

/* ************************************************************************** */
class ReadStartLine : public IState
{
public:
  ReadStartLine(Client* client);
  ~ReadStartLine();
  void run();

private:
  enum ParseState
  {
    ParseMethod,
    ParseUri,
    ParseVersion
  };

  ParseState _parseState;
  std::size_t _iStart;

  void _parseMethod();
  void _parseUri();
  void _parseVersion();
};

#endif
