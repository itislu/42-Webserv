#pragma once
#ifndef STATE_READ_START_LINE_HPP
#define STATE_READ_START_LINE_HPP

#include "client/Client.hpp"
#include "http/states/IState.hpp"
#include <cstddef>

class Client;

/* ************************************************************************** */
class ReadStartLine : public IState
{
public:
  explicit ReadStartLine(Client* client);
  ~ReadStartLine();
  void run();

private:
  enum ParseState
  {
    ParseMethod,
    ParseUri,
    ParseVersion
  };

  ReadStartLine();
  ReadStartLine(const ReadStartLine& other);
  ReadStartLine& operator=(const ReadStartLine& other);

  ParseState _parseState;
  std::size_t _iStart;

  void _parseMethod();
  void _parseUri();
  void _parseVersion();
};

#endif
