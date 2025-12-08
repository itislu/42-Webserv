#pragma once
#ifndef PROCESS_CGI_RESPONSE_HPP
#define PROCESS_CGI_RESPONSE_HPP

#include <libftpp/memory.hpp>
#include <utils/buffer/SmartBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

class Client;
class CgiContext;

class ProcessCgiResponse : public IState<CgiContext>
{
public:
  explicit ProcessCgiResponse(CgiContext* context);

  void run();

private:
  void _readCgiResponse();

  static Logger& _log;

  Client* _client;
  ft::shared_ptr<SmartBuffer> _smartBuffer;
  bool _done;
};

#endif
