#pragma once
#ifndef PARSE_METHOD_HPP
#define PARSE_METHOD_HPP

#include <utils/abnfRules/SequenceRule.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

class Client;
class ReadRequestLine;

/* ************************************************************************** */
class ParseMethod : public IState<ReadRequestLine>
{
public:
  explicit ParseMethod(ReadRequestLine* context);

  void run();

private:
  void _init();
  void _extractMethod();
  bool _methodNotImplemented();

  Client* _client;
  SequenceRule _sequence;
  static Logger& _log;
};

#endif
