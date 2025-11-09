#include "ReadRequestLine.hpp"
#include "ParseMethod.hpp"

#include <client/Client.hpp>
#include <http/StatusCode.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <http/states/readHeaderLines/ReadHeaderLines.hpp>
#include <http/states/writeStatusLine/WriteStatusLine.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>
#include <utils/state/StateHandler.hpp>

/* ************************************************************************** */
// INIT

Logger& ReadRequestLine::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

ReadRequestLine::ReadRequestLine(Client* context)
  : IState(context)
  , _stateHandler(this)
{
  _log.info() << "ReadRequestLine\n";
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
    _log.info() << "ReadRequestLine result\n"
                << getContext()->getRequest().toString() << "\n"
                << "buffer: \n"
                << getContext()->getInBuff().toString() << "\n";
    if (getContext()->getResponse().getStatusCode() == StatusCode::Ok) {
      getContext()->getStateHandler().setState<ReadHeaderLines>();
    } else {
      getContext()->getStateHandler().setState<PrepareResponse>();
    }
    return;
  }
}

StateHandler<ReadRequestLine>& ReadRequestLine::getStateHandler()
{
  return _stateHandler;
}

/* ************************************************************************** */
// PRIVATE
