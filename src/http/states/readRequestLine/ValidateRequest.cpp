#include "ValidateRequest.hpp"

#include <http/states/readRequestLine/ReadRequestLine.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

/* ************************************************************************** */
// PUBLIC

ValidateRequest::ValidateRequest(ReadRequestLine* context)
  : IState<ReadRequestLine>(context)
  , _client(context->getContext())
  , _log(&Logger::getInstance(logFiles::http))
{
  _log->info() << "ValidateRequest\n";
}

void ValidateRequest::run()
{
  //todo
  /*
    - check if target is available
    - check if method is available for target
  */
  getContext()->getStateHandler().setDone();
}

/* ************************************************************************** */
// PRIVATE
