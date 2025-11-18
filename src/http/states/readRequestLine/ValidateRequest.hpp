#pragma once
#include <string>
#include <vector>
#ifndef VALIDATE_REQUEST_HPP
#define VALIDATE_REQUEST_HPP

#include "http/Request.hpp"
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

class Client;
class ReadRequestLine;

/* ************************************************************************** */
class ValidateRequest : public IState<ReadRequestLine>
{
public:
  explicit ValidateRequest(ReadRequestLine* context);

  void run();

private:
  static std::string getRequestPath(const std::string& uri,
                                    const std::string& root);
  static bool validateMethod(const std::vector<std::string>& allowedMethods,
                             Request::Method method);
  static int validateRequestPath(const std::string& path);
  static int validateFile(const std::string& path);
  static int validateDirectory(const std::string& path);

  Client* _client;
  static Logger& _log;
};

#endif
