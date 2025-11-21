
#include "ValidatePost.hpp"
#include "http/states/validateRequest/ValidateRequest.hpp"
#include "utils/state/IState.hpp"

ValidatePost::ValidatePost(ValidateRequest* context)
  : IState<ValidateRequest>(context)
  , _client(context->getContext())
{
  _log.info() << "Validate POST\n";
}

void ValidatePost::run()
{
  // todo
  getContext()->getStateHandler().setDone();
}
