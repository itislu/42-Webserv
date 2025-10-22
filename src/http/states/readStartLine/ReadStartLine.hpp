#pragma once
#ifndef STATE_READ_START_LINE_HPP
#define STATE_READ_START_LINE_HPP

#include <utils/state/StateHandler.hpp>
#include <utils/state/IState.hpp>

class Client;

/* ************************************************************************** */
class ReadStartLine : public IState<Client>
{
public:
  explicit ReadStartLine(Client* context);
  ~ReadStartLine();
  void run();

  StateHandler<ReadStartLine>& getStateHandler();

private:
  StateHandler<ReadStartLine> _stateHandler;

  ReadStartLine();
  ReadStartLine(const ReadStartLine& other);
  ReadStartLine& operator=(const ReadStartLine& other);

};

#endif
