#include "PrepareResponse.hpp"

#include <client/Client.hpp>
#include <http/Request.hpp>
#include <http/states/prepareResponse/HandleGet.hpp>
#include <http/states/writeStatusLine/WriteStatusLine.hpp>
#include <utils/logger/LoggerHandler.hpp>
#include <utils/state/IState.hpp>
#include <utils/state/StateHandler.hpp>

#include <string>

/* ************************************************************************** */
// PUBLIC

PrepareResponse::PrepareResponse(Client* context)
  : IState<Client>(context)
  , _client(context)
  , _log(&LoggerHandler::getInstance(log::http))
  , _stateHandler(this)
  , _initialized(false)
{
  _log->info("PrepareResponse");
}

void PrepareResponse::run()
{
  if (!_initialized) {
    _init();
  }

  _stateHandler.setStateHasChanged(true);
  while (!_stateHandler.isDone() && _stateHandler.stateHasChanged()) {
    _stateHandler.setStateHasChanged(false);
    _stateHandler.getState()->run();
  }

  if (_stateHandler.isDone()) {
    _client->getStateHandler().setState<WriteStatusLine>();
  }
}

StateHandler<PrepareResponse>& PrepareResponse::getStateHandler()
{
  return _stateHandler;
}

/* ************************************************************************** */
// PRIVATE

void PrepareResponse::_init()
{
  _initialized = true;
  // todo on status code error -> HandleError
  const Request::Method method = _client->getRequest().getMethod();
  switch (method) {
    case Request::GET:
      _stateHandler.setState<HandleGet>();
      break;
    default:
      // todo bad request
      // add error body to response
      getContext()->getStateHandler().setState<WriteStatusLine>();
      break;
  }
}
