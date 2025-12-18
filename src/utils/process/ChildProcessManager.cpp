#include <sys/types.h>

#include "ChildProcessManager.hpp"

#include <libftpp/algorithm.hpp>
#include <utils/logger/Logger.hpp>

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <vector>

/* ************************************************************************** */
// INIT

Logger& ChildProcessManager::_log = Logger::getInstance(LOG_SERVER);

/* ************************************************************************** */
// PUBLIC

ChildProcessManager& ChildProcessManager::getInstance()
{
  static ChildProcessManager instance;
  return instance;
}

ChildProcessManager::ChildProcessManager() {}

ChildProcessManager::~ChildProcessManager()
{
  _log.info() << "ChildProcessManager ends\n";
  for (PidVector::iterator it = _children.begin(); it != _children.end();
       ++it) {
    ::kill(*it, SIGKILL);
    ::waitpid(*it, 0, 0);
    _log.info() << "Child(" << *it << ") collected\n";
  }
}

void ChildProcessManager::collectChildren()
{
  std::size_t index = 0;
  while (index < _children.size()) {
    bool remove = false;
    int status = 0;
    const pid_t pid = _children[index];
    const pid_t result = waitpid(pid, &status, WNOHANG);
    if (result == 0) {
      _log.warning() << "Child(" << pid << ") alive\n";
    } else if (result == pid) {
      if (WIFEXITED(status)) {
        _log.info() << "Child(" << pid
                    << ") collected, Exit code: " << WEXITSTATUS(status)
                    << "\n";
      } else {
        _log.warning() << "Child(" << pid << ") collected, no exitcode\n";
      }
      remove = true;
    } else {
      _log.error() << "Child(" << pid
                   << ") error waitpid: " << std::strerror(errno) << "\n";
      remove = true;
    }

    if (remove) {
      _children.erase(_children.begin() +
                      static_cast<PidVector::difference_type>(index));
    } else {
      index++;
    }
  }
}

void ChildProcessManager::addChild(pid_t pid)
{
  if (pid <= 0) {
    return;
  }
  if (ft::contains(_children, pid)) {
    return;
  }

  _log.info() << "Child(" << pid << ") created\n";
  _children.push_back(pid);
}

void ChildProcessManager::waitForChild(pid_t pid)
{
  if (pid <= 0) {
    return;
  }

  ::waitpid(pid, 0, 0);

  _log.info() << "Child(" << pid << ") collected\n";

  const PidVector::iterator iter =
    std::find(_children.begin(), _children.end(), pid);
  if (iter != _children.end()) {
    _children.erase(iter);
  }
}

void ChildProcessManager::killChild(pid_t pid)
{
  if (pid <= 0) {
    return;
  }
  const PidVector::const_iterator iter =
    std::find(_children.begin(), _children.end(), pid);
  if (iter != _children.end()) {
    _log.info() << "Child(" << pid << ") killed\n";
    ::kill(pid, SIGKILL);
  }
}
