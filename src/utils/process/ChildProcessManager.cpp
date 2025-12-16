#include "ChildProcessManager.hpp"

#include <libftpp/algorithm.hpp>
#include <utils/logger/Logger.hpp>

#include <algorithm>
#include <cerrno>
#include <csignal>
#include <cstddef>
#include <cstring>
#include <signal.h>
#include <sys/signal.h>
#include <sys/types.h>
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
  for (PidVector::iterator it = _childs.begin(); it != _childs.end(); ++it) {
    ::kill(*it, SIGTERM);
    ::waitpid(*it, 0, 0);
    _log.info() << "Child(" << *it << ") collected\n";
  }
}

void ChildProcessManager::collectChilds()
{
  std::size_t index = 0;
  while (index < _childs.size()) {
    bool remove = false;
    int status = 0;
    const pid_t pid = _childs[index];
    const pid_t result = waitpid(pid, &status, WNOHANG);
    if (result == pid) {
      if (WIFEXITED(status)) {
        _log.info() << "Child(" << pid
                    << ") collected, Exit code: " << WEXITSTATUS(status)
                    << "\n";
      }
      remove = true;
    } else if (result < 0) {
      _log.info() << "Child(" << pid << ") error waitpid: " << strerror(errno)
                  << "\n";
      remove = true;
    }

    if (remove) {
      _childs.erase(_childs.begin() +
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
  if (ft::contains(_childs, pid)) {
    return;
  }

  _log.info() << "Child(" << pid << ") created\n";
  _childs.push_back(pid);
}

void ChildProcessManager::waitForChild(pid_t pid)
{
  if (pid <= 0) {
    return;
  }

  ::waitpid(pid, 0, 0);

  _log.info() << "Child(" << pid << ") collected\n";

  const PidVector::iterator iter =
    std::find(_childs.begin(), _childs.end(), pid);
  if (iter != _childs.end()) {
    _childs.erase(iter);
  }
}

void ChildProcessManager::killChild(pid_t pid)
{
  if (pid <= 0) {
    return;
  }
  const PidVector::const_iterator iter =
    std::find(_childs.begin(), _childs.end(), pid);
  if (iter != _childs.end()) {
    _log.info() << "Child(" << pid << ") killed\n";
    ::kill(pid, SIGKILL);
  }
}
