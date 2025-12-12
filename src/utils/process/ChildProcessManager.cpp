#include "ChildProcessManager.hpp"

#include <utils/logger/Logger.hpp>

#include <algorithm>
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <signal.h>
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
  for (std::vector<pid_t>::iterator it = _childs.begin(); it != _childs.end();
       ++it) {
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
    if (result == 0) {
      // child still alive
    } else if (result == pid) {
      if (WIFEXITED(status)) {
        _log.info() << "Child(" << pid << ") collected, ";
        _log.info() << "Exit code: " << WEXITSTATUS(status) << "\n";
      }
      remove = true;
    } else {
      // result == -1
      _log.info() << "Child(" << pid << ") error waitpid: " << strerror(errno);
      remove = true;
    }

    if (remove) {
      _childs.erase(_childs.begin() +
                    static_cast<std::vector<pid_t>::difference_type>(index));
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

  const std::vector<pid_t>::const_iterator iter =
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
  const std::vector<pid_t>::const_iterator iter =
    std::find(_childs.begin(), _childs.end(), pid);
  if (iter != _childs.end()) {
    ::kill(pid, SIGTERM);
    _log.info() << "Child(" << pid << ") killed\n";
    _childs.erase(iter);
  }
}
