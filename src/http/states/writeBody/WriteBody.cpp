#include "WriteBody.hpp"

#include <client/Client.hpp>
#include <utils/IBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <fstream>
#include <sstream>

/* ************************************************************************** */
// INIT

Logger& WriteBody::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

WriteBody::WriteBody(Client* context)
  : IState<Client>(context)
  , _client(context)
{
  _log.info() << "WriteBody\n";
  _log.info() << getContext()->getRequest().toString() << "\n";
}

void WriteBody::run()
{
  std::ifstream& ifs = _client->getResponse().getBody();

  if (!ifs.is_open()) {
    _log.error() << "Failed to open file\n";
    _client->getStateHandler().setDone();
    return;
  }

  std::ostringstream oss;
  oss << ifs.rdbuf();

  IBuffer& outBuffer = _client->getOutBuff();
  outBuffer.append(oss.str());

  // _log.info(_client->getOutBuff().toString());
  _client->getStateHandler().setDone();
}

/* ************************************************************************** */
// PRIVATE
