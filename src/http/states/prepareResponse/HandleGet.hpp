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
  void _addContentLengthHeader();
  void _addContentType();
  void _openFile();
  static std::string _generateAutoindex(const std::string& path);

  PrepareResponse* _prepareResponse;
  Client* _client;
  static Logger& _log;
};

#endif
