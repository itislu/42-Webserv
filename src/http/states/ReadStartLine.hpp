#pragma once
#ifndef STATE_READ_START_LINE_HPP
#define STATE_READ_START_LINE_HPP

#include "client/Client.hpp"
#include "http/states/IState.hpp"
#include <cstddef>

namespace {
enum ParseState
{
  ParseMethod,
  ParseUri,
  ParseVersion
};
}

/* ************************************************************************** */
class ReadStartLine : public IState
{
public:
  ReadStartLine();
  ~ReadStartLine();
  ReadStartLine(const ReadStartLine& other);
  ReadStartLine& operator=(const ReadStartLine& other);

  void run(Client& client);

private:
  ParseState _parseState;
  std::size_t _iStart;

  void _parseMethod(Client& client);
  void _parseUri(Client& client);
  void _parseVersion(Client& client);
};

#endif // STATE_READ_FIRST_LINE_HPP
