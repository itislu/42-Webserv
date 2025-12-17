#pragma once
#ifndef CHILD_PROCESS_MANAGER_HPP
#define CHILD_PROCESS_MANAGER_HPP

#include <sys/types.h>

#include <utils/logger/Logger.hpp>

#include <vector>

/* ************************************************************************** */
class ChildProcessManager
{
public:
  typedef std::vector<pid_t> PidVector;

  static ChildProcessManager& getInstance();
  ~ChildProcessManager();

  void collectChilds();
  void addChild(pid_t pid);
  void waitForChild(pid_t pid);
  void killChild(pid_t pid);

private:
  ChildProcessManager();
  ChildProcessManager(const ChildProcessManager& other);
  ChildProcessManager& operator=(const ChildProcessManager& other);

  static Logger& _log;
  PidVector _childs;
};

#endif
