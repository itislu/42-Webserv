#include "HandleError.hpp"

#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

/* ************************************************************************** */
// INIT

Logger& HandleError::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

HandleError::HandleError(PrepareResponse* context)
  : IState<PrepareResponse>(context)
  , _prepareResponse(context)
  , _client(_prepareResponse->getContext())
{
  _log.info() << "HandleError\n";
}

void HandleError::run()
{
  getContext()->getStateHandler().setDone();
}

/* ************************************************************************** */
// PRIVATE
