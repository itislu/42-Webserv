#include "ParseVersion.hpp"
#include "utils/Buffer.hpp"

#include <http/http.hpp>
#include <client/Client.hpp>
#include <http/states/readStartLine/ReadStartLine.hpp>
#include <string>
#include <utils/Done.hpp>
#include <utils/IState.hpp>

#include <cstddef>

/* ************************************************************************** */
// PUBLIC

ParseVersion::ParseVersion(ReadStartLine* context)
  : IState<ReadStartLine>(context)
  , _client(context->getContext())
{
  _finder.initTokenFinder(&_client->getInBuff(), http::CRLF);
}

ParseVersion::~ParseVersion() {}

/* ************************************************************************** */
// PRIVATE

ParseVersion::ParseVersion()
  : IState<ReadStartLine>(NULL)
  , _client(NULL)
{
}

ParseVersion::ParseVersion(const ParseVersion& other)
  : IState<ReadStartLine>(other.getContext())
  , _client(other._client)
{
  *this = other;
}

ParseVersion& ParseVersion::operator=(const ParseVersion& other)
{
  if (this != &other) {
    // todo copy logic
  }
  return *this;
}

void ParseVersion::run()
{
  if (!_finder.buffContainsToken()) {
    return;
  }
  std::string strVersion = _finder.getStrUntilToken();
  _client->getRequest().setVersion(strVersion);

  // TODO validate Version --> OK/ALARM

  // Remove from buffer
  Buffer& buff = _client->getInBuff();
  buff.remove(strVersion.size() + _finder.getTokenSize());

  getContext()->getStateHandler().setDone();
}
