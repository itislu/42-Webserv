#include "WriteBody.hpp"

#include <client/Client.hpp>
#include <utils/Buffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <fstream>
#include <sstream>

/* ************************************************************************** */
// PUBLIC

WriteBody::WriteBody(Client* context)
  : IState<Client>(context)
  , _client(context)
  , _log(Logger::getInstance(logFiles::http))
{
  _log.info() << "WriteBody\n";
  _log.info() << getContext()->getRequest().toString() << "\n";
}

void WriteBody::run()
{
  const std::ifstream& ifs = _client->getResponse().getInputFileStream();

  if (!ifs.is_open()) {
    _log.error() << "Failed to open file\n";
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
