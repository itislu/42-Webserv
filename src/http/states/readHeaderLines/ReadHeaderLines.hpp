#pragma once
#ifndef READ_HEADER_LINES_HPP
#define READ_HEADER_LINES_HPP

#include "RequestHeaderValidator.hpp"
#include <http/utils/HeaderParser.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <cstddef>

class Client;

/* ************************************************************************** */
class ReadHeaderLines : public IState<Client>
{
public:
  explicit ReadHeaderLines(Client* context);

  void run();

  static void setMaxFieldLineSize(std::size_t value);
  static void setMaxHeaderSize(std::size_t value);
  static void resetMaxFieldLineSize();
  static void resetMaxHeaderSize();

private:
  void _init();
  void _readLines();
  void _setNextState();

  static Logger& _log;
  static const std::size_t _defaultMaxFieldLineSize = 8192;
  static const std::size_t _defaultMaxHeaderSize = 8192;
  static std::size_t _maxFieldLineSize;
  static std::size_t _maxHeaderSize;

  Client* _client;
  HeaderParser _headerParser;
  bool _done;
};

#endif
