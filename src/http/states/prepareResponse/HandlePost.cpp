#include "HandlePost.hpp"

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
  // todo
  getContext()->getStateHandler().setDone();
}

/* ************************************************************************** */
// PRIVATE
