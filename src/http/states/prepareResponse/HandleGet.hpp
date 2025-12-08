#pragma once
#include "utils/buffer/SmartBuffer.hpp"
#ifndef HANDLE_GET_HPP
#define HANDLE_GET_HPP

#include <utils/state/IState.hpp>

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
  void _handleAutoIndex();
  void _handleStaticFile();
  static void _generateAutoindex(const std::string& path, SmartBuffer& buffer);

  static Logger& _log;
  PrepareResponse* _prepareResponse;
  Client* _client;
};

#endif
