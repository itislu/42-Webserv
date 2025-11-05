#include "HandleDelete.hpp"

#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

/* ************************************************************************** */
// PUBLIC

HandleDelete::HandleDelete(PrepareResponse* context)
  : IState<PrepareResponse>(context)
  , _prepareResponse(context)
  , _client(_prepareResponse->getContext())
  , _log(&Logger::getInstance(logFiles::http))
{
  _log->info() << "HandleDelete\n";
}

void HandleDelete::run()
{
  getContext()->getStateHandler().setDone();
}

/* ************************************************************************** */
// PRIVATE
