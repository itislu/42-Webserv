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

  if (isDirectory(_path)) {
    endState(StatusCode::Forbidden);
    return;
  }
  if (!isFile(_path)) {
    endState(StatusCode::NotFound);
    return;
  }
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

/*
  204 No Content if deleted successfully
  404 Not Found if the file doesn’t exist
  403 Forbidden if not allowed
  409 Conflict if the directory isn’t empty (optional)
  500 Internal Server Error if deletion fails unexpectedly
*/
