#include "HandlePost.hpp"

#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/StatusCode.hpp>
#include <http/states/prepareResponse/HandleError.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <http/states/writeStatusLine/WriteStatusLine.hpp>
#include <libftpp/string.hpp>
#include <utils/fileUtils.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <string>

/* ************************************************************************** */
// INIT

Logger& HandlePost::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

HandlePost::HandlePost(PrepareResponse* context)
  : IState<PrepareResponse>(context)
  , _prepareResponse(context)
  , _client(_prepareResponse->getContext())
{
  _log.info() << "HandlePost\n";
}

void HandlePost::run()
{
  _createData();
  _setNextState();
}

/* ************************************************************************** */
// PRIVATE

void HandlePost::_setNextState()
{
  const StatusCode& statusCode = _client->getResponse().getStatusCode();

  if (statusCode == StatusCode::Ok || statusCode == StatusCode::Created) {
    getContext()->getStateHandler().setDone();
  } else {
    getContext()->getStateHandler().setState<HandleError>();
  }
}

std::string HandlePost::_getFileName(const std::string& directory)
{
  std::string filename;
  const int maxCount = 100;
  for (int i = 0; i < maxCount; ++i) {
    filename.clear();
    filename.append(directory);
    filename.append(getRandomFileName("post_"));
    if (!isFile(filename)) {
      return filename;
    }
  }
  _log.error() << "HandlePost: no filename found\n";
  _client->getResponse().setStatusCode(StatusCode::InternalServerError);
  return filename;
}

void HandlePost::_createData()
{
  const std::string directory = _client->getResource().getPath();

  _log.info() << "HandlePost: " << directory << '\n';

  const std::string newFilePath = _getFileName(directory);
  _client->getRequest().getBody().moveBufferToFile(newFilePath);
  _client->getResponse().setStatusCode(StatusCode::Created);
  Headers& headers = _client->getResponse().getHeaders();
  headers.addHeader("Location", newFilePath);
  headers.addHeader("Content-Length", "0");
}
