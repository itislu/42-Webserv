#include "WriteBody.hpp"
#include "http/StatusCode.hpp"

#include <client/Client.hpp>
#include <sstream>
#include <utils/IBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <fstream>
#include <ios>

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
}

// NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
void WriteBody::run()
try {
  std::ifstream& ifs = _client->getResponse().getBody();
  _log.info() << "WriteBody: run\n";

  if (!ifs.is_open()) {
    _log.error() << "Failed to open file\n";
    _client->getStateHandler().setDone();
    return;
  }

  IBuffer& outBuffer = _client->getOutBuff();
  if (outBuffer.size() >= _outBufferLimit) {
    return;
  }

  IBuffer::RawBytes buff(_chunkSize);
  ifs.read(buff.data(), _chunkSize);
  const std::streamsize readCount = ifs.gcount();
  if (readCount > 0) {
    outBuffer.append(buff, readCount);
  }

  if (ifs.eof()) {
    _log.info() << "WriteBody: done\n";
    _client->getStateHandler().setDone();
  }
} catch (const IBuffer::BufferException& e) {
  _log.error() << "WriteBody: " << e.what() << '\n';
  getContext()->getResponse().setStatusCode(StatusCode::InternalServerError);
  getContext()->getStateHandler().setDone();
}
// NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)

/* ************************************************************************** */
// PRIVATE
