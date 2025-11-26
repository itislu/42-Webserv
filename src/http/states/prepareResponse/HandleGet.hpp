#pragma once
#ifndef HANDLE_GET_HPP
#define HANDLE_GET_HPP

#include "utils/state/IState.hpp"

#include <string>

class Logger;
class Client;
class PrepareResponse;

/* ************************************************************************** */
class HandleGet : public IState<PrepareResponse>
{
public:
  explicit HandleGet(PrepareResponse* context);

  void run();

private:
  void _setNextState();
  std::string _getResource();
  void _addContentLengthHeader();
  void _addContentType();
  void _openFile();
  bool _fail();

  PrepareResponse* _prepareResponse;
  Client* _client;
  static Logger& _log;
};

#endif
