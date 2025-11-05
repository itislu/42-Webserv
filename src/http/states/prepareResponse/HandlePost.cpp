#include "HandlePost.hpp"

#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

/* ************************************************************************** */
// PUBLIC

HandlePost::HandlePost(PrepareResponse* context)
  : IState<PrepareResponse>(context)
  , _prepareResponse(context)
  , _client(_prepareResponse->getContext())
  , _log(&Logger::getInstance(logFiles::http))
{
  _log->info() << "HandlePost\n";
}

void HandlePost::run()
{
  getContext()->getStateHandler().setDone();
}

/* ************************************************************************** */
// PRIVATE
