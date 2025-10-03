#include "ReadStartLine.hpp"
#include "utils/Buffer.hpp"

#include <client/Client.hpp>
#include <http/Request.hpp>
#include <http/http.hpp>
#include <http/states/IState.hpp>

#include <cstddef>
#include <string>

/* ************************************************************************** */
// PUBLIC

ReadStartLine::ReadStartLine(Client* client)
  : IState(client)
  , _sizeStartLine(0)
  , _parseState(InitParseMethod)
{
}

ReadStartLine::~ReadStartLine() {}

/**
 * @brief ### Read the first line of a HTTP Request
 *
 * Example: `GET test/hello_world.html HTTP/1.1\r\n`
 */
void ReadStartLine::run()
{
  if (_parseState == Init) {
    _initNextState(ParseMethod);
  }
  if (_parseState == ParseMethod) {
    _parseMethod();
  }
  if (_parseState == ParseUriScheme) {
    _parseUri();
  }
  if (_parseState == ParseVersion) {
    _parseVersion();
  }
  if (_parseState == Done) {
    // todo change state
  }
}

/* ************************************************************************** */
// PRIVATE

ReadStartLine::ReadStartLine()
  : IState(NULL)
  , _sizeStartLine(0)
  , _parseState(ParseMethod)
{
}

ReadStartLine::ReadStartLine(const ReadStartLine& other)
  : IState(NULL)
  , _sizeStartLine(0)
  , _parseState(ParseMethod)
{
  *this = other;
}

ReadStartLine& ReadStartLine::operator=(const ReadStartLine& other)
{
  if (this != &other) {
  }
  return *this;
}

void ReadStartLine::_initNextState(ParseState nextState)
{
  switch (nextState) {
    case ParseMethod:
      _finder.initTokenFinder(&getClient()->getInBuff(), http::SP);
      break;
    case ParseUriScheme:
      _finder.initTokenFinder(&getClient()->getInBuff(), http::SP);
      break;
    case ParseVersion:
      _finder.initTokenFinder(&getClient()->getInBuff(), http::CRLF);
    default:
      return;
  }
  _parseState = nextState;
}

void ReadStartLine::_parseMethod()
{
  if (!_finder.buffContainsToken()) {
    if (getClient()->getInBuff().getSize() > Request::MAX_METHOD_LEN) {
      // TODO error method is longer than possible
    }
    return;
  }

  std::string strMethod = _finder.getStrUntilToken();
  const Request::Method method = Request::strToMethod(strMethod);
  getClient()->getRequest().setMethod(method);

  // TODO validate method --> OK/ALARM

  Buffer& buff = getClient()->getInBuff();
  buff.remove(strMethod.size() + _finder.getTokenSize());
  _parseState = InitParseUri;
}

void ReadStartLine::_initParseUri()
{
  _finder.initTokenFinder(&getClient()->getInBuff(), http::SP);
  _parseState = ParseUri;
}

void ReadStartLine::_parseUri()
{
  if (!_finder.buffContainsToken()) {
    return;
  }
  std::string rawUri = _finder.getStrUntilToken();
  getClient()->getRequest().getUri().setRaw(rawUri);

  // TODO validate URI --> OK/ALARM

  Buffer& buff = getClient()->getInBuff();
  buff.remove(rawUri.size() + _finder.getTokenSize());
  _parseState = InitParseVersion;
}

void ReadStartLine::_initParseVersion()
{
  _finder.initTokenFinder(&getClient()->getInBuff(), http::CRLF);
  _parseState = ParseVersion;
}

void ReadStartLine::_parseVersion()
{
  if (!_finder.buffContainsToken()) {
    return;
  }
  std::string strVersion = _finder.getStrUntilToken();
  getClient()->getRequest().setVersion(strVersion);

  // TODO validate Version --> OK/ALARM

  // Remove from buffer
  Buffer& buff = getClient()->getInBuff();
  buff.remove(strVersion.size() + _finder.getTokenSize());
  _parseState = Done;
}
