#include "ValidateDelete.hpp"
#include "http/states/validateRequest/ValidateRequest.hpp"
#include "utils/state/IState.hpp"

ValidateDelete::ValidateDelete(ValidateRequest* context)
  : IState<ValidateRequest>(context)
  , _client(context->getContext())
{
  _log.info() << "Validate DELETE\n";
}

void ValidateDelete::run()
{
  // todo
  getContext()->getStateHandler().setDone();
}
