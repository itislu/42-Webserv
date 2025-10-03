#pragma once
#ifndef STATE_READ_START_LINE_HPP
#define STATE_READ_START_LINE_HPP

#include <cstddef>
#include "http/states/IState.hpp"
#include "http/states/readStartLineState/IReadStartLineState.hpp"

class Client;

/* ************************************************************************** */
class ReadStartLine : public IState
{
public:
  explicit ReadStartLine(Client* client);
  ~ReadStartLine();
  void run();

private:
  IReadStartLineState* state;

  ReadStartLine();
  ReadStartLine(const ReadStartLine& other);
  ReadStartLine& operator=(const ReadStartLine& other);

  std::size_t _sizeStartLine;

  void _parseMethod();
  void _parseUri();
  void _parseVersion();
};

#endif
