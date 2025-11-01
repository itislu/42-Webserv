#include "ReadRequestLine.hpp"
#include "ParseMethod.hpp"

#include <client/Client.hpp>
#include <http/states/readHeaderLines/ReadHeaderLines.hpp>
#include <utils/state/IState.hpp>
#include <utils/state/StateHandler.hpp>

/* ************************************************************************** */
// PUBLIC

ReadRequestLine::ReadRequestLine(Client* context)
  : IState(context)
  , _stateHandler(this)
{
  _stateHandler.setState<ParseMethod>();
}

/**
 * @brief ### Read the first line of a HTTP request
 *
 * Example: `GET test/hello_world.html HTTP/1.1\r\n`
 */
void ReadRequestLine::run()
{
  _stateHandler.setStateHasChanged(true);
  while (!_stateHandler.isDone() && _stateHandler.stateHasChanged()) {
    _stateHandler.setStateHasChanged(false);
    _stateHandler.getState()->run();
  }

  if (_stateHandler.isDone()) {
    getContext()->getStateHandler().setState<ReadHeaderLines>();
    return;
  }
}

StateHandler<ReadRequestLine>& ReadRequestLine::getStateHandler()
{
  return _stateHandler;
}

/* ************************************************************************** */
// PRIVATE
