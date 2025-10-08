#pragma once
#ifndef STATE_READ_START_LINE_HPP
#define STATE_READ_START_LINE_HPP

#include <cstddef>
#include "utils/StateHandler.hpp"
#include <utils/IState.hpp>

class Client;

/* ************************************************************************** */
class ReadStartLine : public IState<Client>
{
public:
  explicit ReadStartLine(Client* client);
  ~ReadStartLine();
  void run();

  StateHandler<ReadStartLine>& getStateHandler();

private: 
  StateHandler<ReadStartLine> _stateHandler;

  ReadStartLine();
  ReadStartLine(const ReadStartLine& other);
  ReadStartLine& operator=(const ReadStartLine& other);

  // bool isDone() const;
  std::size_t _sizeStartLine;
};

// #include "ReadStartLine.tpp" 

#endif
