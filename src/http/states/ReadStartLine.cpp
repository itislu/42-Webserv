#include "http/states/ReadStartLine.hpp"
#include "client/Client.hpp"
#include "http/Request.hpp"
#include "http/http.hpp"
#include "utils/tryGetStrUntil.hpp"
#include <stdexcept>

/* ************************************************************************** */
// PUBLIC

ReadStartLine::ReadStartLine()
  : _iStart(0)
{
  _parseState = ParseMethod;
}

ReadStartLine::~ReadStartLine() {}

ReadStartLine::ReadStartLine(const ReadStartLine& other)
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

/**
 * @brief ### Read the first line of a HTTP request
 *
 * Example: `GET test/hello_world.html HTTP/1.1`
 */
void ReadStartLine::run(Client& client)
{
  if (_parseState == ParseMethod){
      _parseMethod(client);
  }
  if (_parseState == ParseUri){
      _parseUri(client);
  }
  if (_parseState == ParseVersion){
      _parseVersion(client);
  }
}

/* ************************************************************************** */
// PRIVATE

void ReadStartLine::_parseMethod(Client& client)
{
  Client::Buffer buff = client.getBuffer();
  Client::Buffer tokens;

  tokens.push_back(http::SP); // todo const ??
  try {
    std::string strMethod = tryGetStrUntil(client.getBuffer(), _iStart, tokens);
    _iStart += strMethod.size() + tokens.size();
    request::Method method = Request::strToMethod(strMethod);
    client.getRequest().setMethod(method);
    if (method == request::UNDEFINED) {
      // TODO ALAAAAARM "method not defined/available"
      return;
    }
    _parseState = ParseUri;
  } catch (std::out_of_range e) {
    return; // no space found
  }
}

void ReadStartLine::_parseUri(Client& client)
{
  Client::Buffer buff = client.getBuffer();
  Client::Buffer tokens;

  tokens.push_back(http::SP); // todo const ??
  try {
    std::string rawUri = tryGetStrUntil(client.getBuffer(), _iStart, tokens);
    _iStart += rawUri.size() + tokens.size();
    client.getRequest().getUri().setRaw(rawUri);
    _parseState = ParseVersion;

    // TODO validate URI --> OK/ALARM
  } catch (std::out_of_range e) {
    return; // no space found
  }
}

void ReadStartLine::_parseVersion(Client& client)
{
  Client::Buffer buff = client.getBuffer();
  Client::Buffer tokens;

  tokens.push_back(http::CR); // todo const ??
  tokens.push_back(http::LF); // todo const ??
  try {
    std::string strVersion =
      tryGetStrUntil(client.getBuffer(), _iStart, tokens);
    _iStart += strVersion.size() + tokens.size();
    client.getRequest().setVersion(strVersion);

    //TODO validate Version --> OK/ALARM
  } catch (std::out_of_range e) {
    return; // no space found
  }
}
