#pragma once
#ifndef PARSE_URI_HPP
#define PARSE_URI_HPP

#include "http/TokenFinder.hpp"
#include "http/Uri.hpp"
#include <utils/IState.hpp>

class Client;
class ReadStartLine;

/* ************************************************************************** */
class ParseUri : public IState<ReadStartLine>
{
public:
  explicit ParseUri(ReadStartLine* context);
  ~ParseUri();

  void run();

private:
  enum ParseUriState
  {
    ParseScheme,
    ParseAuthority,
    ParsePath,
    ParseQuery,
    ParseFragment
  };

  ParseUri();
  ParseUri(const ParseUri& other);
  ParseUri& operator=(const ParseUri& other);

  Client* _client;
  Uri _tmpUri;
  TokenFinder _finder;


  static bool _isValidChr(unsigned char chr);
  static bool _isReservedChr(unsigned char chr);
  static bool _isUnreservedChr(unsigned char chr);

  void _parseRawUri();
  void _parseScheme();
  void _parseAuthority();
  void _parsePath();
  void _parseQuery();
  void _parseFragment();
  
};

#endif // PARSE_URI_HPP
