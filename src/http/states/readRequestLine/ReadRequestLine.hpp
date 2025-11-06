#pragma once
#ifndef READ_REQUEST_LINE_HPP
#define READ_REQUEST_LINE_HPP

#include <utils/logger/Logger.hpp>
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
  Logger& _log; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
};

#endif
