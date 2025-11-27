
#include "ValidatePost.hpp"
#include "utils/fileUtils.hpp"

#include <client/Client.hpp>
#include <http/Resource.hpp>
#include <http/StatusCode.hpp>
#include <http/states/validateRequest/ValidateRequest.hpp>
#include <libftpp/utility.hpp>
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
  /* TODO: check order/codes of validations - with testing */

  if (_location != FT_NULLPTR && _location->isCgi()) {
    validateCGI();
    return;
  }

  validateStaticPost();
}

void ValidatePost::validateCGI()
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
  /* TODO: CGI Post doesn't need write permission? */
  validateParentDirPermissions();
}

void ValidatePost::validateStaticPost()
{
  validateParentDirPermissions();
}

void ValidatePost::validateParentDirPermissions()
{
  std::string dirPath = _path.substr(0, _path.find_last_of('/'));
  if (dirPath.empty()) {
    dirPath = "/";
  }
  if (!isDirectory(dirPath)) {
    endState(StatusCode::NotFound);
    return;
  }
  if (!isExecuteable(dirPath)) {
    endState(StatusCode::NotFound);
    return;
  }
  if (!isWriteable(dirPath)) {
    endState(StatusCode::Forbidden);
    return;
  }
  endState(StatusCode::Ok);
}
void ValidatePost::endState(StatusCode::Code status)
{
  _client->getResponse().setStatusCode(status);
  if (status != StatusCode::Ok) {
    _client->getResource().setType(Resource::Error);
  }
}
