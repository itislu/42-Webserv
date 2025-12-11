#include "WaitForCgi.hpp"

#include <client/Client.hpp>
#include <http/CgiContext.hpp>
#include <http/StatusCode.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <http/states/readBody/ReadBody.hpp>
#include <http/states/writeBody/WriteBody.hpp>
#include <http/states/writeStatusLine/WriteStatusLine.hpp>
#include <libftpp/memory.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <cstdlib>
#include <exception>

/* ************************************************************************** */
// INIT

Logger& WaitForCgi::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

WaitForCgi::WaitForCgi(Client* context)
  : IState<Client>(context)
  , _client(context)
{
  _log.info() << *_client << " WaitForCgi\n";
}

void WaitForCgi::run()
try {
  if (_client->getCgiContext()->getShProcessCgiResponse().isDone()) {
    _log.info() << *_client << " WaitForCgi done\n";
    _client->getCgiContext().reset();
    _client->getStateHandler().setState<WriteStatusLine>();
  } else if (_client->getResponse().getStatusCode() != StatusCode::Ok) {
    getContext()->getStateHandler().setState<PrepareResponse>();
  }
} catch (const std::exception& e) {
  _log.error() << *_client << " WaitForCgi: " << e.what() << "\n";
  getContext()->getResponse().setStatusCode(StatusCode::InternalServerError);
  getContext()->getStateHandler().setState<PrepareResponse>();
}
