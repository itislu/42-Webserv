#include "WriteBody.hpp"

#include <client/Client.hpp>
#include <libftpp/algorithm.hpp>
#include <utils/Buffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/logger/LoggerHandler.hpp>
#include <utils/state/IState.hpp>

#include <fstream>
#include <sstream>
#include <string>

/* ************************************************************************** */
// PUBLIC

WriteBody::WriteBody(Client* context)
  : IState<Client>(context)
  , _client(context)
  , _log(LoggerHandler::getInstance(log::http))
{
  _log.info("WriteBody");
  _log.info(getContext()->getRequest().toString());
}

void WriteBody::run()
{
  const std::ifstream& ifs = _client->getResponse().getInputFileStream();

  if (!ifs.is_open()) {
    _log.error("Failed to open file");
  }

  std::ostringstream oss;
  oss << ifs.rdbuf();

  Buffer& outBuffer = _client->getOutBuff();
  outBuffer.add(oss.str());

  // _log.info(_client->getOutBuff().toString());
  _client->getStateHandler().setDone();
}

/* ************************************************************************** */
// PRIVATE
