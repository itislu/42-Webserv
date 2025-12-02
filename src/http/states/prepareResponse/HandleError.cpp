#include "HandleError.hpp"

#include <client/Client.hpp>
#include <http/StatusCode.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <sstream>
#include <string>

/* ************************************************************************** */
// INIT

Logger& HandleError::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

HandleError::HandleError(PrepareResponse* context)
  : IState<PrepareResponse>(context)
  , _prepareResponse(context)
  , _client(_prepareResponse->getContext())
{
  _log.info() << "HandleError\n";
}

void HandleError::run()
{
  // todo get custom error page from config

  const StatusCode& statuscode = _client->getResponse().getStatusCode();
  _client->getOutBuff().append(_makeErrorBody(statuscode));
  getContext()->getStateHandler().setDone();
}

/* ************************************************************************** */
// PRIVATE

std::string HandleError::_makeErrorBody(const StatusCode& statuscode)
{
  const int code = statuscode.getCode();
  const std::string& reason = statuscode.getReason();

  std::ostringstream oss;

  oss << "<!DOCTYPE html>\n";
  oss << "<html>\n";
  oss << "<head><title>" << code << " " << reason << "</title></head>\n";
  oss << "<body>\n";
  oss << "<h1>" << code << " " << reason << "</h1>\n";
  oss << "<p>Something went wrong.</p>\n";
  oss << "</body>\n</html>\n";

  return oss.str();
}
