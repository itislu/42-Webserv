#pragma once
#ifndef EXECUTE_CGI_HPP
#define EXECUTE_CGI_HPP

#include <client/Client.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <cstddef>
#include <string>
#include <vector>

class CgiContext;

class ExecuteCgi : public IState<CgiContext>
{
  enum State
  {
    PrepareEnv,
    ExecuteScript,
    ProvideBody,
    Done
  };

public:
  explicit ExecuteCgi(CgiContext* context);

  void run();

private:
  void _prepareEnv();
  void _addEnvVar(const std::string& key, const std::string& value);
  std::vector<char*> _buildEnvp();
  void _executeScript();
  void _handleChild();
  void _provideBody();

  static Logger& _log;

  Client* _client;
  State _state;
  std::vector<std::string> _env;
  std::size_t _contentLength;
  std::size_t _bytesWritten;
};

#endif
