#include "HandleError.hpp"
#include "http/Headers.hpp"

#include <client/Client.hpp>
#include <http/Resource.hpp>
#include <http/Response.hpp>
#include <http/StatusCode.hpp>
#include <http/headerUtils.hpp>
#include <http/states/prepareResponse/PrepareResponse.hpp>
#include <libftpp/memory.hpp>
#include <libftpp/optional.hpp>
#include <utils/buffer/MemoryBuffer.hpp>
#include <utils/buffer/StaticFileBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <exception>
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
try {
  _resetResponse();

  const Response& response = _client->getResponse();
  const Resource& resource = _client->getResource();
  const ft::optional<std::string> optErrPage =
    resource.getErrorPage(response.getStatusCode().getCode());
  if (optErrPage.has_value()) {
    _log.info() << "HandleError: custom error page: " << *optErrPage << '\n';
    _customErrorPage(*optErrPage);
  } else {
    _defaultErrorPage();
  }

  getContext()->getStateHandler().setDone();
} catch (const std::exception& e) {
  _log.error() << *_client << " HandleError: " << e.what() << '\n';
  _client->getResponse().setStatusCode(StatusCode::InternalServerError);
  throw;
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

/**
 * @brief Reset response i case some state before already set Headers.
 *
 * - for example CGI
 */
void HandleError::_resetResponse()
{
  Response& response = _client->getResponse();
  response.getHeaders() = Headers();
}

void HandleError::_customErrorPage(const std::string& errPage)
{
  Response& response = _client->getResponse();
  response.setBody(ft::make_shared<StaticFileBuffer>(errPage));
  setContentLengthHeader(response.getHeaders(), response.getBody()->size());
  setContentTypeHeader(response.getHeaders(), errPage);
}

void HandleError::_defaultErrorPage()
{
  Response& response = _client->getResponse();

  // set body
  const std::string bodyData = _makeErrorBody(response.getStatusCode());
  response.setBody(ft::make_shared<MemoryBuffer>(bodyData));

  // set headers
  setContentLengthHeader(response.getHeaders(), bodyData.length());
  response.getHeaders().setHeader(header::contentType, "text/html");
}
