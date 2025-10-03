#include "ParseMethod.hpp"
#include "client/Client.hpp"
#include "http/states/readStartLineState/IReadStartLineState.hpp"
#include "http/states/readStartLineState/ReadStartLine.hpp"
#include "utils/Buffer.hpp"
#include <http/Request.hpp>
#include <http/http.hpp>
#include <string>

/* ************************************************************************** */
// PUBLIC

ParseMethod::ParseMethod(ReadStartLine* state)
  : IReadStartLineState(state)
  , _client(state->getClient())
{
  _finder.initTokenFinder(&state->getClient()->getInBuff(), http::SP);
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
}

/* ************************************************************************** */
// PRIVATE
