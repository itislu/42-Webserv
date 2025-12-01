#include "WriteBody.hpp"

#include <client/Client.hpp>
#include <http/StatusCode.hpp>
#include <http/states/prepareResponse/HandleError.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
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
  , _done(false)
{
  _log.info() << "WriteBody\n";
}

void WriteBody::run()
try {
  _writeIntoOutBuffer();

  if (_done || _fail()) {
    getContext()->getStateHandler().setDone();
  }
} catch (const IBuffer::BufferException& e) {
  _log.error() << "WriteBody: " << e.what() << '\n';
  getContext()->getResponse().setStatusCode(StatusCode::InternalServerError);
  getContext()->getStateHandler().setDone();
}

/* ************************************************************************** */
// PRIVATE

bool WriteBody::_fail()
{
  return _client->getResponse().getStatusCode() != StatusCode::Ok;
}

void WriteBody::_writeIntoOutBuffer()
{
  std::ifstream& ifs = _client->getResponse().getBody();
  _log.info() << "WriteBody: run\n";

  if (!ifs.is_open()) {
    _log.info() << "no body\n";
    _done = true;
    return;
  }

  IBuffer& outBuffer = _client->getOutBuff();
  if (outBuffer.size() >= _outBufferLimit) {
    return;
  }

  IBuffer::RawBytes buff(_chunkSize);
  ifs.read(buff.data(), _chunkSize);
  if (ifs.fail() && !ifs.eof()) {
    _log.error() << "WriteBody: stream read error\n";
    _client->getResponse().setStatusCode(StatusCode::InternalServerError);
    return;
  }

  const std::streamsize readCount = ifs.gcount();
  if (readCount > 0) {
    outBuffer.append(buff, readCount);
  }

  if (ifs.eof()) {
    _done = true;
  }
}
