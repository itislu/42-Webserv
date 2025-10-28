#pragma once
#ifndef READ_START_LINE_HPP
#define READ_START_LINE_HPP

#include <utils/state/IState.hpp>
#include <utils/state/StateHandler.hpp>

class Client;

/* ************************************************************************** */
class ReadStartLine : public IState<Client>
{
public:
  explicit ReadStartLine(Client* context);

  void run();

  StateHandler<ReadStartLine>& getStateHandler();

private:
  StateHandler<ReadStartLine> _stateHandler;
};

#endif
