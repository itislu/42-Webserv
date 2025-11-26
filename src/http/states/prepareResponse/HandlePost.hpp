#pragma once
#ifndef HANDLE_POST_HPP
#define HANDLE_POST_HPP

#include <utils/state/IState.hpp>

#include <string>

class Logger;
class Client;
class PrepareResponse;

/* ************************************************************************** */
class HandlePost : public IState<PrepareResponse>
{
public:
  explicit HandlePost(PrepareResponse* context);

  void run();

private:
  static std::string _getFileName(const std::string& directory);

  void _createData();

  static Logger& _log;

  PrepareResponse* _prepareResponse;
  Client* _client;
};

#endif
