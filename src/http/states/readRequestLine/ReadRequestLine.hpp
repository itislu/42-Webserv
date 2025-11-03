#pragma once
#ifndef READ_REQUEST_LINE_HPP
#define READ_REQUEST_LINE_HPP

#include <utils/state/IState.hpp>
#include <utils/state/StateHandler.hpp>

class Client;

/* ************************************************************************** */
class ReadRequestLine : public IState<Client>
{
public:
  explicit ReadRequestLine(Client* context);

  void run();

  StateHandler<ReadRequestLine>& getStateHandler();

private:
  StateHandler<ReadRequestLine> _stateHandler;
};

#endif
