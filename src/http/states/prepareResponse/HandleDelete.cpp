#include "HandleDelete.hpp"

#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

/* ************************************************************************** */
// INIT

Logger& HandleDelete::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

HandleDelete::HandleDelete(PrepareResponse* context)
  : IState<PrepareResponse>(context)
  , _prepareResponse(context)
  , _client(_prepareResponse->getContext())
{
  _log.info() << "HandleDelete\n";
}

void HandleDelete::run()
{
  // todo
  getContext()->getStateHandler().setDone();
}

/* ************************************************************************** */
// PRIVATE
