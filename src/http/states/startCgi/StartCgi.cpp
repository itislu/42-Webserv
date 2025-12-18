#include "StartCgi.hpp"

#include <client/Client.hpp>
#include <event/CgiReadEventHandler.hpp>
#include <event/CgiWriteEventHandler.hpp>
#include <event/EventManager.hpp>
#include <http/CgiContext.hpp>
#include <http/StatusCode.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <http/states/readBody/ReadBody.hpp>
#include <libftpp/memory.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <cstdlib>
#include <exception>

/* ************************************************************************** */
// INIT

Logger& StartCgi::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

StartCgi::StartCgi(Client* context)
  : IState<Client>(context)
  , _client(context)
{
  _log.info() << *_client << " StartCgi\n";
}

void StartCgi::run()
try {
  _client->getCgiContext() = ft::make_shared<CgiContext>(_client);
  _client->getStateHandler().setState<ReadBody>();
} catch (const std::exception& e) {
  _log.error() << *_client << " StartCgi: " << e.what() << "\n";
  _client->getResponse().setStatusCode(StatusCode::InternalServerError);
  _client->getStateHandler().setState<PrepareResponse>();
}

/* ************************************************************************** */
// PRIVATE
