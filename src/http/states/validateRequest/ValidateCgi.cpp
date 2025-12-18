#include "ValidateCgi.hpp"

#include <client/Client.hpp>
#include <http/Resource.hpp>
#include <http/StatusCode.hpp>
#include <http/states/validateRequest/ValidateRequest.hpp>
#include <utils/fileUtils.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <string>

/* ************************************************************************** */
// INIT

Logger& ValidateCgi::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

ValidateCgi::ValidateCgi(ValidateRequest* context)
  : IState<ValidateRequest>(context)
  , _client(context->getContext())
  , _path(context->getPath())
{
  _log.info() << "Validate CGI\n";
}

void ValidateCgi::run()
{
  validate();
  getContext()->getStateHandler().setDone();
}

void ValidateCgi::validate()
{
  _client->getResource().setType(Resource::Cgi);

  if (!isExecuteable(_path)) {
    endState(StatusCode::Forbidden);
    return;
  }
  endState(StatusCode::Ok);
}

void ValidateCgi::endState(StatusCode::Code status)
{
  _client->getResponse().setStatusCode(status);
  if (status != StatusCode::Ok) {
    _client->getResource().setType(Resource::Error);
  }
}
