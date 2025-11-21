
#include "ValidatePost.hpp"
#include "client/Client.hpp"
#include "config/FileUtils.hpp"
#include "http/StatusCode.hpp"
#include "http/states/validateRequest/ValidateRequest.hpp"
#include "libftpp/utility.hpp"
#include "utils/logger/Logger.hpp"
#include "utils/state/IState.hpp"
#include <string>

/* ************************************************************************** */
// INIT

Logger& ValidateRequest::_log = Logger::getInstance(LOG_HTTP);

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
  if (config::fileutils::isDirectory(_path)) {
    endState(StatusCode::Forbidden);
    return;
  }
  if (!config::fileutils::isFile(_path)) {
    endState(StatusCode::NotFound);
    return;
  }
  if (!config::fileutils::isExecuteable(_path)) {
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
  if (!config::fileutils::isDirectory(dirPath)) {
    endState(StatusCode::NotFound);
    return;
  }
  if (!config::fileutils::isExecuteable(dirPath)) {
    endState(StatusCode::NotFound);
    return;
  }
  if (!config::fileutils::isWriteable(dirPath)) {
    endState(StatusCode::Forbidden);
    return;
  }
}
void ValidatePost::endState(StatusCode::Code status)
{
  _client->getResponse().setStatusCode(status);
}
