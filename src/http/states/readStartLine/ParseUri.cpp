#include "ParseUri.hpp"
#include "utils/Buffer.hpp"

#include <cctype>
#include <client/Client.hpp>
#include <http/http.hpp>
#include <http/states/readStartLine/ParseVersion.hpp>
#include <http/states/readStartLine/ReadStartLine.hpp>
#include <utils/IState.hpp>
#include <utils/StateHandler.hpp>

#include <cstddef>
#include <string>

/* ************************************************************************** */
// PUBLIC

ParseUri::ParseUri(ReadStartLine* context)
  : IState<ReadStartLine>(context)
  , _client(context->getContext())
{
  _finder.initTokenFinder(&_client->getInBuff(), http::SP);
}

ParseUri::~ParseUri() {}

/* ************************************************************************** */
// PRIVATE

ParseUri::ParseUri()
  : IState<ReadStartLine>(NULL)
  , _client(NULL)
{
}

ParseUri::ParseUri(const ParseUri& other)
  : IState<ReadStartLine>(other.getContext())
  , _client(other._client)
{
  *this = other;
}

ParseUri& ParseUri::operator=(const ParseUri& other)
{
  if (this != &other) {
    // todo copy logic
  }
  return *this;
}

void ParseUri::run()
{
  if (!_finder.buffContainsToken()) {
    return;
  }
  std::string rawUri = _finder.getStrUntilToken();
  _tmpUri.setRaw(rawUri);

  _parseRawUri(); // TODO validate URI --> OK/ALARM

  Buffer& buff = _client->getInBuff();
  buff.remove(rawUri.size() + _finder.getTokenSize());

  _client->getRequest().setUri(_tmpUri);
  getContext()->getStateHandler().setState<ParseVersion>();
}

// bool ParseUri::_isValidChr(unsigned char chr)
// {
//   // Unreserved: ALPHA / DIGIT / "-" / "." / "_" / "~"
//   if (_isUnreservedChr(chr)) {
//     return true;
//   }

//   // Reserved: gen-delims / sub-delims
//   if (_isReservedChr(chr)) {
//     return true;
//   }

//   // '%' starts percent-encoding
//   if (chr == '%') {
//     return true;
//   }

//   // ' ' end of line
//   if (chr == ' ') {
//     return true;
//   }

//   return false;
// }

// bool ParseUri::_isReservedChr(unsigned char chr)
// {
//   // Reserved: gen-delims / sub-delims
//   const std::string reserved = ":/?#[]@!$&'()*+,;=";
//   return reserved.find(static_cast<char>(chr)) != std::string::npos;
// }

// bool ParseUri::_isUnreservedChr(unsigned char chr)
// {
//   // Unreserved: ALPHA / DIGIT / "-" / "." / "_" / "~"
//   return (std::isalnum(chr) != 0 || chr == '-' || chr == '.' || chr == '_' ||
//           chr == '~');
// }

void ParseUri::_parseRawUri()
{
  //TODO
  _parseScheme();
  _parseAuthority();
  _parsePath();
  _parseQuery();
  _parseFragment();
}

void ParseUri::_parseScheme()
{
  const Buffer& buff = _client->getInBuff();
  (void)buff;
}

void ParseUri::_parseAuthority()
{

}

void ParseUri::_parsePath()
{

}

void ParseUri::_parseQuery()
{

}

void ParseUri::_parseFragment()
{

}
