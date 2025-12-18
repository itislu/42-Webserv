
#include "ValidatePost.hpp"
#include "libftpp/string.hpp"

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

Logger& ValidatePost::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

ValidatePost::ValidatePost(ValidateRequest* context)
  : IState<ValidateRequest>(context)
  , _client(context->getContext())
  , _path(context->getPath())
  , _server(context->getServer())
  , _location(context->getLocation())
{
  _log.info() << "Validate POST\n";
}

void ValidatePost::run()
{
  validate();
  getContext()->getStateHandler().setDone();
}

void ValidatePost::validate()
{
  if (isDirectory(_path)) {
    if (!isWriteable(_path) || !isExecuteable(_path)) {
      endState(StatusCode::Forbidden);
      return;
    }
    if (!ft::ends_with(_path, '/')) {
      _path.append("/");
      _client->getResource().setPath(_path);
    }
    endState(StatusCode::Ok);
    return;
  }

  if (isFile(_path)) {
    endState(StatusCode::Conflict);
  } else {
    endState(StatusCode::NotFound);
  }
}

void ValidatePost::endState(StatusCode::Code status)
{
  _client->getResponse().setStatusCode(status);
  if (status != StatusCode::Ok) {
    _client->getResource().setType(Resource::Error);
  }
}
