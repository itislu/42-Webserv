#include "http/states/ReadStartLine.hpp"
#include "client/Client.hpp"
#include "http/Request.hpp"
#include "http/http.hpp"
#include "http/states/IState.hpp"
#include "utils/tryGetStrUntil.hpp"
#include <cstddef>
#include <stdexcept>
#include <string>

/* ************************************************************************** */
// PUBLIC

ReadStartLine::ReadStartLine(Client* client)
  : IState(client)
  , _parseState(ParseMethod)
  , _iStart(0)
{
}

ReadStartLine::~ReadStartLine() {}

/**
 * @brief ### Read the first line of a HTTP Request
 *
 * Example: `GET test/hello_world.html HTTP/1.1`
 */
void ReadStartLine::run()
{
  if (_parseState == ParseMethod) {
    _parseMethod();
  }
  if (_parseState == ParseUri) {
    _parseUri();
  }
  if (_parseState == ParseVersion) {
    _parseVersion();
  }
}

/* ************************************************************************** */
// PRIVATE

ReadStartLine::ReadStartLine()
  : IState(NULL)
  , _parseState(ParseMethod)
  , _iStart(0)
{
}

ReadStartLine::ReadStartLine(const ReadStartLine& other)
  : IState(NULL)
  , _parseState(ParseMethod)
  , _iStart(0)
{
  *this = other;
}

ReadStartLine& ReadStartLine::operator=(const ReadStartLine& other)
{
  if (this != &other) {
    // todo copy logic
  }
  return *this;
}

void ReadStartLine::_parseMethod()
{
  const Client::Buffer buff = getClient()->getBuffer();
  Client::Buffer tokens;

  tokens.push_back(http::SP); // todo const ??
  try {
    std::string strMethod = tryGetStrUntil(buff, _iStart, tokens);
    _iStart += strMethod.size() + tokens.size();
    const Request::Method method = Request::strToMethod(strMethod);
    getClient()->getRequest().setMethod(method);
    if (method == Request::UNDEFINED) {
      // TODO ALAAAAARM "method not defined/available"
      return;
    }
    _parseState = ParseUri;
  } catch (const std::out_of_range& e) {
    return; // no space found
  }
}

void ReadStartLine::_parseUri()
{
  const Client::Buffer buff = getClient()->getBuffer();
  Client::Buffer tokens;

  tokens.push_back(http::SP); // todo const ??
  try {
    std::string rawUri = tryGetStrUntil(buff, _iStart, tokens);
    _iStart += rawUri.size() + tokens.size();
    getClient()->getRequest().getUri().setRaw(rawUri);
    _parseState = ParseVersion;

    // TODO validate URI --> OK/ALARM
  } catch (const std::out_of_range& e) {
    return; // no space found
  }
}

void ReadStartLine::_parseVersion()
{
  const Client::Buffer buff = getClient()->getBuffer();
  Client::Buffer tokens;

  tokens.push_back(http::CR); // todo const ??
  tokens.push_back(http::LF); // todo const ??
  try {
    std::string strVersion = tryGetStrUntil(buff, _iStart, tokens);
    _iStart += strVersion.size() + tokens.size();
    getClient()->getRequest().setVersion(strVersion);

    // TODO validate Version --> OK/ALARM
  } catch (const std::out_of_range& e) {
    return; // no space found
  }
}
