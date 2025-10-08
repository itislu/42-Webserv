#include "ParseMethod.hpp"

#include <utils/IState.hpp>
#include <http/states/readStartLine/ParseUri.hpp>
#include <http/states/readStartLine/ReadStartLine.hpp>
#include <client/Client.hpp>
#include <http/Request.hpp>
#include <http/http.hpp>
#include <utils/Buffer.hpp>
#include <utils/StateHandler.hpp>

#include <string>

/* ************************************************************************** */
// PUBLIC

ParseMethod::ParseMethod(ReadStartLine* context)
  : IState<ReadStartLine>(context)
  , _client(context->getContext())
{
  _finder.initTokenFinder(&_client->getInBuff(), http::SP);
}

ParseMethod::~ParseMethod() {}

void ParseMethod::run()
{
  if (!_finder.buffContainsToken()) {
    if (_client->getInBuff().getSize() > Request::MAX_METHOD_LEN) {
      // TODO error -> method is longer than longest method
    }
    return;
  }

  std::string strMethod = _finder.getStrUntilToken();
  const Request::Method method = Request::strToMethod(strMethod);
  _client->getRequest().setMethod(method);

  // TODO validate method --> OK/ALARM

  Buffer& buff = _client->getInBuff();
  buff.remove(strMethod.size() + _finder.getTokenSize());

  getContext()->getStateHandler().setState<ParseUri>();
}

/* ************************************************************************** */
// PRIVATE
