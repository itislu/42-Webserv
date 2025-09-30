#include "http/states/ReadStartLine.hpp"
#include "client/Client.hpp"
#include "http/Request.hpp"
#include "http/http.hpp"
#include "http/states/IState.hpp"
#include "utils/tryGetStrUntil.hpp"
#include <stdexcept>

/* ************************************************************************** */
// PUBLIC

ReadStartLine::ReadStartLine(Client* client)
  : IState(client)
{
  _parseState = ParseMethod;
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

void ReadStartLine::_parseMethod()
{
  Client::Buffer buff = client->getBuffer();
  Client::Buffer tokens;

  tokens.push_back(http::SP); // todo const ??
  try {
    std::string strMethod =
      tryGetStrUntil(client->getBuffer(), _iStart, tokens);
    _iStart += strMethod.size() + tokens.size();
    Request::Method method = Request::strToMethod(strMethod);
    client->getRequest().setMethod(method);
    if (method == Request::UNDEFINED) {
      // TODO ALAAAAARM "method not defined/available"
      return;
    }
    _parseState = ParseUri;
  } catch (std::out_of_range e) {
    return; // no space found
  }
}

void ReadStartLine::_parseUri()
{
  Client::Buffer buff = client->getBuffer();
  Client::Buffer tokens;

  tokens.push_back(http::SP); // todo const ??
  try {
    std::string rawUri = tryGetStrUntil(client->getBuffer(), _iStart, tokens);
    _iStart += rawUri.size() + tokens.size();
    client->getRequest().getUri().setRaw(rawUri);
    _parseState = ParseVersion;

    // TODO validate URI --> OK/ALARM
  } catch (std::out_of_range e) {
    return; // no space found
  }
}

void ReadStartLine::_parseVersion()
{
  Client::Buffer buff = client->getBuffer();
  Client::Buffer tokens;

  tokens.push_back(http::CR); // todo const ??
  tokens.push_back(http::LF); // todo const ??
  try {
    std::string strVersion =
      tryGetStrUntil(client->getBuffer(), _iStart, tokens);
    _iStart += strVersion.size() + tokens.size();
    client->getRequest().setVersion(strVersion);

    // TODO validate Version --> OK/ALARM
  } catch (std::out_of_range e) {
    return; // no space found
  }
}
