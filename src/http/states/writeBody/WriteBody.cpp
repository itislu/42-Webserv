#include "WriteBody.hpp"

#include <client/Client.hpp>
#include <http/StatusCode.hpp>
#include <http/states/prepareResponse/HandleError.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <exception>

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

  if (_done) {
    _log.info() << *_client << " WriteBody: " << "done\n";
    getContext()->getStateHandler().setDone();
  }
} catch (const std::exception& e) {
  _log.error() << *_client << " WriteBody: " << e.what() << '\n';
  getContext()->getResponse().setStatusCode(StatusCode::InternalServerError);
  throw;
}

/* ************************************************************************** */
// PRIVATE

void WriteBody::_writeIntoOutBuffer()
{
  /** // todo
   * for chunked encoding:
   * - _client->getOutBuffQueue().getSmartBuffer()
   * - add response body to smartbuffer
   */

  if (_client->getResponse().getBody() != FT_NULLPTR) {
    _client->getOutBuffQueue().append(_client->getResponse().getBody());
  }
  _done = true;
}
