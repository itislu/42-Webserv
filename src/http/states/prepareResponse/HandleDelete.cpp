#include "HandleDelete.hpp"

#include <client/Client.hpp>
#include <http/StatusCode.hpp>
#include <http/states/prepareResponse/HandleError.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <cstdio>
#include <string>

/* ************************************************************************** */
// INIT

Logger& HandleDelete::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

HandleDelete::HandleDelete(PrepareResponse* context)
  : IState<PrepareResponse>(context)
  , _prepareResponse(context)
  , _client(_prepareResponse->getContext())
{
  _log.info() << "HandleDelete\n";
}

void HandleDelete::run()
{
  _deleteFile();
  _setNextState();
}

/* ************************************************************************** */
// PRIVATE

void HandleDelete::_setNextState()
{
  const StatusCode& statusCode = _client->getResponse().getStatusCode();
  if (statusCode == StatusCode::Ok) {
    getContext()->getStateHandler().setDone();
  } else {
    getContext()->getStateHandler().setState<HandleError>();
  }
}

void HandleDelete::_deleteFile()
{
  const std::string filepath = _client->getResource().getPath();
  const int res = std::remove(filepath.c_str());
  if (res != 0) {
    _client->getResponse().setStatusCode(StatusCode::InternalServerError);
    _log.error() << "HandleDelete failed to delete file\n";
  }
}
