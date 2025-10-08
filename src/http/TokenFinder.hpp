#pragma once
#ifndef TOKEN_FINDER_HPP
#define TOKEN_FINDER_HPP

#include <cstddef>
#include <string>
#include <utils/Buffer.hpp>

/* ************************************************************************** */
class TokenFinder
{
public:
  void initTokenFinder(Buffer* buffer, char token);
  void initTokenFinder(Buffer* buffer, const char* token);
  std::size_t getTokenSize() const;

  bool buffContainsToken();
  std::string getStrUntilToken() const;

private:
  void _setToken(char token);
  void _setToken(const char* token);

  Buffer* _buffer;
  Buffer::Container _token;
  long _posInBuff;
  long _tokenPos;
};

#endif // PARSER_HPP
