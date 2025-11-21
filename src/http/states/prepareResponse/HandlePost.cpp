#include "HandlePost.hpp"
#include "http/Headers.hpp"
#include "http/StatusCode.hpp"

#include <client/Client.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

/* ************************************************************************** */
// INIT

Logger& HandlePost::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

HandlePost::HandlePost(PrepareResponse* context)
  : IState<PrepareResponse>(context)
  , _prepareResponse(context)
  , _client(_prepareResponse->getContext())
{
  _log.info() << "HandlePost\n";
}

void HandlePost::run()
{
  Headers& headers = _client->getRequest().getHeaders();
  headers.addHeader("Content-Type", "text/plain");
  getContext()->getStateHandler().setDone();
}

/* ************************************************************************** */
// PRIVATE
