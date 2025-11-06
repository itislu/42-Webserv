#pragma once
#ifndef HANDLE_GET_HPP
#define HANDLE_GET_HPP

#include "utils/state/IState.hpp"

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
  void _addContentLengthHeader();
  void _addContentType();
  void _openFile();

  PrepareResponse* _prepareResponse;
  Client* _client;
  Logger* _log;
};

#endif
