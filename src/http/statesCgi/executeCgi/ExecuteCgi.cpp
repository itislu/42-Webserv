#include <sys/types.h>

#include "ExecuteCgi.hpp"

#include <client/Client.hpp>
#include <http/Headers.hpp>
#include <http/Request.hpp>
#include <http/Resource.hpp>
#include <http/StatusCode.hpp>
#include <http/headerUtils.hpp>
#include <libftpp/array.hpp>
#include <libftpp/string.hpp>
#include <libftpp/utility.hpp>
#include <utils/Pipe.hpp>
#include <utils/buffer/IBuffer.hpp>
#include <utils/buffer/IInOutBuffer.hpp>
#include <utils/logger/Logger.hpp>
#include <utils/state/IState.hpp>

#include <algorithm>
#include <cassert>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <vector>

/* ************************************************************************** */
// INIT

Logger& ExecuteCgi::_log = Logger::getInstance(LOG_HTTP);

/* ************************************************************************** */
// PUBLIC

ExecuteCgi::ExecuteCgi(CgiContext* context)
  : IState<CgiContext>(context)
  , _client(context->getClient())
  , _state(PrepareEnv)
  , _contentLength(0)
  , _bytesWritten(0)
{
  _log.info() << *_client << " ExecuteCgi\n";
}

void ExecuteCgi::run()
try {
  if (_state == PrepareEnv) {
    _prepareEnv();
  }
  if (_state == ExecuteScript) {
    _executeScript();
  }
  if (_state == ProvideBody) {
    _provideBody();
  }
  if (_state == Done) {
    _log.info() << *_client << " ExecuteCgi done\n";
    getContext()->getShExecCgi().setDone();
  }
} catch (const std::exception& e) {
  _log.error() << *_client << " ExecuteCgi: " << e.what() << "\n";
  _client->getResponse().setStatusCode(StatusCode::InternalServerError);
  throw;
}

/* ************************************************************************** */
// PRIVATE

void ExecuteCgi::_prepareEnv()
{
  if (!getContext()->contentLengthAvailable()) {
    return;
  }

  Request& request = _client->getRequest();
  const Resource& resource = _client->getResource();
  const Headers& reqHeaders = request.getHeaders();

  _contentLength = getContext()->getContentLength();

  _addEnvVar("GATEWAY_INTERFACE", "CGI/1.1");
  _addEnvVar("SERVER_PROTOCOL", "HTTP/1.1");
  _addEnvVar("REQUEST_METHOD", request.getStrMethod());
  _addEnvVar("CONTENT_LENGTH", ft::to_string(_contentLength));
  if (reqHeaders.contains(header::contentType)) {
    _addEnvVar("CONTENT_TYPE", reqHeaders.at(header::contentType));
  }
  _addEnvVar("SCRIPT_NAME", resource.getPath());
  _addEnvVar("QUERY_STRING", request.getUri().getQuery());
  _state = ExecuteScript;
}

void ExecuteCgi::_addEnvVar(const std::string& key, const std::string& value)
{
  std::string entry(key);
  entry.append("=");
  entry.append(value);
  _env.push_back(entry);
}

// NOLINTBEGIN(cppcoreguidelines-pro-type-const-cast)
std::vector<char*> ExecuteCgi::_buildEnvp()
{
  std::vector<char*> envpOut;
  envpOut.clear();
  envpOut.reserve(_env.size() + 1);

  for (size_t i = 0; i < _env.size(); ++i) {
    envpOut.push_back(const_cast<char*>(_env[i].c_str()));
  }

  envpOut.push_back(NULL);
  return envpOut;
}
// NOLINTEND(cppcoreguidelines-pro-type-const-cast)

void ExecuteCgi::_executeScript()
{
  Pipe& pipeToCgi = getContext()->getPipeClientToCgi();
  Pipe& pipeFromCgi = getContext()->getPipeCgiToClient();

  const pid_t pid = fork();
  if (pid < 0) {
    throw std::runtime_error("fork failed");
  }
  if (pid == 0) {
    _handleChild();
  }

  _log.info() << "Run cgi script\n";
  getContext()->setChildPid(pid);

  // Close child's ends
  pipeToCgi.closeRead();
  pipeFromCgi.closeWrite();

  _state = ProvideBody;
}

// NOLINTBEGIN(cppcoreguidelines-pro-type-const-cast)
void ExecuteCgi::_handleChild()
try {
  const Resource& resource = _client->getResource();
  Pipe& pipeToCgi = getContext()->getPipeClientToCgi();
  Pipe& pipeFromCgi = getContext()->getPipeCgiToClient();

  // Redirect stdin
  if (dup2(pipeToCgi.getReadFd(), STDIN_FILENO) == -1) {
    throw std::runtime_error(std::string("dup2(stdin) failed: ") +
                             strerror(errno));
  }
  pipeToCgi.close();

  // Redirect stdout
  if (dup2(pipeFromCgi.getWriteFd(), STDOUT_FILENO) == -1) {
    throw std::runtime_error(std::string("dup2(stdout) failed: ") +
                             strerror(errno));
  }
  pipeFromCgi.close();

  const std::string& interpreter = resource.getLocation()->getCgiPass();
  const std::string& script = resource.getPath();
  ft::array<char* const, 3> args = {
    const_cast<char*>(interpreter.c_str()),
    const_cast<char*>(script.c_str()),
    FT_NULLPTR,
  };
  execve(interpreter.c_str(), args.data(), _buildEnvp().data());
  throw std::runtime_error(std::string("execve failed: ") + strerror(errno));
} catch (const std::exception& e) {
  _log.error() << "CgiChild: exception: " << e.what() << '\n';
  std::exit(1);
}
// NOLINTEND(cppcoreguidelines-pro-type-const-cast)

void ExecuteCgi::_provideBody()
{
  Request& request = _client->getRequest();
  IInOutBuffer& body = request.getBody();
  Pipe& pipeToCgi = getContext()->getPipeClientToCgi();

  if (!body.isEmpty()) {
    // Write TO CHILD (stdin)
    const std::size_t toWrite = std::min(body.size(), Client::maxChunk);
    IBuffer::RawBytes msg = body.getRawBytes(0, toWrite);
    const ssize_t res = write(pipeToCgi.getWriteFd(), msg.data(), msg.size());
    if (res < 0) {
      throw std::runtime_error("ExecuteCgi: write failed");
    }
    if (res > 0) {
      body.removeFront(res);
    }
    _bytesWritten += res;
  }
  if (_bytesWritten >= _contentLength) {
    pipeToCgi.closeWrite();
    _state = Done;
  }
}
