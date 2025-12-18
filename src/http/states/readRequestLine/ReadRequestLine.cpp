#include "ReadRequestLine.hpp"
#include "ParseMethod.hpp"

#include <client/Client.hpp>
#include <http/StatusCode.hpp>
#include <http/states/prepareResponse/HandleError.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <http/states/readHeaderLines/ReadHeaderLines.hpp>
#include <http/states/writeStatusLine/WriteStatusLine.hpp>
#include <utils/buffer/IBuffer.hpp>
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
  _log.info() << *context << " ReadRequestLine\n";
  _stateHandler.setState<ParseMethod>();
}

/**
 * @brief ### Read the first line of a HTTP request
 *
 * Example: `GET test/hello_world.html HTTP/1.1\r\n`
 */
void ReadRequestLine::run()
try {
  _stateHandler.setStateHasChanged(true);
  while (!_stateHandler.isDone() && _stateHandler.stateHasChanged()) {
    _stateHandler.setStateHasChanged(false);
    _stateHandler.getState()->run();
  }

  if (_stateHandler.isDone()) {
    _setNextState();
    return;
  }
} catch (const IBuffer::BufferException& e) {
  _log.error() << *getContext() << " ReadRequestLine: " << e.what() << '\n';
  getContext()->getResponse().setStatusCode(StatusCode::InternalServerError);
  getContext()->getStateHandler().setState<PrepareResponse>();
}

StateHandler<ReadRequestLine>& ReadRequestLine::getStateHandler()
{
  return _stateHandler;
}

/* ************************************************************************** */
// PRIVATE

void ReadRequestLine::_setNextState()
{
  const StatusCode& statusCode = getContext()->getResponse().getStatusCode();

  if (statusCode.is2xxCode()) {
    getContext()->getStateHandler().setState<ReadHeaderLines>();
  } else {
    getContext()->getStateHandler().setState<PrepareResponse>();
  }
}
