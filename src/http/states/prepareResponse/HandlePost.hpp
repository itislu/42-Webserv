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
  std::string _getFileName(const std::string& directory);

  void _setNextState();
  void _createData();
  std::string _createNoRootLocation(const std::string& absLocation);

  static Logger& _log;

  PrepareResponse* _prepareResponse;
  Client* _client;
};

#endif
