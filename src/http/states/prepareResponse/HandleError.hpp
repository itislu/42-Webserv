#pragma once
#ifndef HANDLE_ERROR_HPP
#define HANDLE_ERROR_HPP

#include <http/StatusCode.hpp>
#include <utils/state/IState.hpp>

#include <string>

class Logger;
class Client;
class PrepareResponse;

/* ************************************************************************** */
class HandleError : public IState<PrepareResponse>
{
public:
  explicit HandleError(PrepareResponse* context);

  void run();

private:
  static std::string _makeErrorBody(const StatusCode& statuscode);

  PrepareResponse* _prepareResponse;
  Client* _client;
  static Logger& _log;
};

#endif
