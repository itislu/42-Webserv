#include "TokenFinder.hpp"
#include "utils/Buffer.hpp"
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <string>

/* ************************************************************************** */
// PUBLIC

void TokenFinder::initTokenFinder(Buffer* buffer, char token)
{
  _buffer = buffer;
  _tokenPos = -1;
  _posInBuff = 0;
  _setToken(token);
}

void TokenFinder::initTokenFinder(Buffer* buffer, const char* token)
{
  _buffer = buffer;
  _tokenPos = -1;
  _posInBuff = 0;
  _setToken(token);
}

std::size_t TokenFinder::getTokenSize() const
{
  return _token.size();
}

bool TokenFinder::buffContainsToken()
{
  if (_tokenPos != -1) {
    return true;
  }

  const Buffer::iterator iter = std::search(_buffer->begin() + _posInBuff,
                                            _buffer->end(),
                                            _token.begin(),
                                            _token.end());

  _posInBuff = std::distance(_buffer->begin(), iter);
  if (iter != _buffer->end()) {
    _tokenPos = _posInBuff;
    return true;
  }
  return false;
}

std::string TokenFinder::getStrUntilToken() const
{
  return std::string(_buffer->begin(), _buffer->begin() + _tokenPos);
}

/* ************************************************************************** */
// PRIVATE

void TokenFinder::_setToken(char token)
{
  _token.clear();
  _token.push_back(token);
}

void TokenFinder::_setToken(const char* token)
{
  std::string str(token);
  _token.clear();
  _token = Buffer::Container(str.begin(), str.end());
}
