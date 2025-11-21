
#include "ValidateGet.hpp"
#include "http/states/validateRequest/ValidateRequest.hpp"
#include "utils/state/IState.hpp"

ValidateGet::ValidateGet(ValidateRequest* context)
  : IState<ValidateRequest>(context)
  , _client(context->getContext())
{
  _log.info() << "Validate GET\n";
}

void ValidateGet::run()
{
  // todo
  getContext()->getStateHandler().setDone();
}