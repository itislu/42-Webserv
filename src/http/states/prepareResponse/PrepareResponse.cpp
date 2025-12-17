#include "PrepareResponse.hpp"
#include "http/states/prepareResponse/HandleRedirect.hpp"

#include <client/Client.hpp>
#include <http/Request.hpp>
#include <http/StatusCode.hpp>
#include <http/states/prepareResponse/HandleDelete.hpp>
#include <http/states/prepareResponse/HandleError.hpp>
#include <http/states/prepareResponse/HandleGet.hpp>
#include <http/states/prepareResponse/HandlePost.hpp>
#include <http/states/writeStatusLine/WriteStatusLine.hpp>
#include <utils/buffer/IBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>
#include <utils/state/StateHandler.hpp>

/* ************************************************************************** */
// INIT

Logger& PrepareResponse::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

PrepareResponse::PrepareResponse(Client* context)
  : IState<Client>(context)
  , _client(context)
  , _stateHandler(this)
{
  _log.info() << *_client << " PrepareResponse\n";
  _init();

  // todo remove this log after testing
  _log.info() << "PrepareResponse Request:\n\n"
              << getContext()->getRequest().toString() << "\n\n";
}

void PrepareResponse::run()
try {
  _stateHandler.setStateHasChanged(true);
  while (!_stateHandler.isDone() && _stateHandler.stateHasChanged()) {
    _stateHandler.setStateHasChanged(false);
    _stateHandler.getState()->run();
  }

  if (_stateHandler.isDone()) {
    _client->getStateHandler().setState<WriteStatusLine>();
  }
} catch (const IBuffer::BufferException& e) {
  _log.error() << *_client << " PrepareResponse: " << e.what() << '\n';
  getContext()->getResponse().setStatusCode(StatusCode::InternalServerError);
  throw;
}

StateHandler<PrepareResponse>& PrepareResponse::getStateHandler()
{
  return _stateHandler;
}

/* ************************************************************************** */
// PRIVATE

void PrepareResponse::_init()
{
  /* TODO: This */
  if (_client->getResponse().getStatusCode() == StatusCode::MovedPermanently) {
    _stateHandler.setState<HandleRedirect>();
  } else if (_client->getResponse().getStatusCode() != StatusCode::Ok) {
    _stateHandler.setState<HandleError>();
  } else {
    const Request::Method method = _client->getRequest().getMethod();
    switch (method) {
      case Request::GET:
        _stateHandler.setState<HandleGet>();
        break;
      case Request::POST:
        _stateHandler.setState<HandlePost>();
        break;
      case Request::DELETE:
        _stateHandler.setState<HandleDelete>();
        break;
      case Request::UNDEFINED:
        _stateHandler.setState<HandleError>();
        break;
    }
  }
}
