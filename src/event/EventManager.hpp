#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include <event/EventHandler.hpp>
#include <libftpp/memory.hpp>
#include <socket/SocketManager.hpp>
#include <utils/logger/Logger.hpp>

#include <map>
#include <vector>

class Client;
class ServerManager;

class EventManager
{
public:
  typedef int RawFd;
  typedef std::map<RawFd, ft::shared_ptr<EventHandler> >::iterator iterHandler;
  typedef std::map<RawFd, ft::shared_ptr<EventHandler> >::const_iterator
    const_iterHandler;

  ~EventManager() {};
  static EventManager& getInstance();
  int check();
  void checkTimeouts();

  EventHandler& getHandler(RawFd fdes) const;
  void addCgiHandler(ft::shared_ptr<EventHandler> handler);

private:
  EventManager() {};
  EventManager(const EventManager& other);
  EventManager& operator=(const EventManager& other);

  static ServerManager& _serverManager();
  static SocketManager& _socketManager();

  /* EVENTS */
  void _checkActivity();
  void _acceptClient(int fdes, unsigned events);
  void _disconnectEventHandler(const EventHandler& handler);

  /* TIMEOUT */
  int _calculateTimeout();
  long _getMinTimeout() const;
  void _getTimedOutHandlers(
    std::vector<ft::shared_ptr<EventHandler> >& timedOut) const;

  /* MANAGE HANDLER */
  void _addHandler(ft::shared_ptr<EventHandler> handler);
  void _removeHandler(RawFd fdes);

  static Logger& _log;

  std::map<RawFd, ft::shared_ptr<EventHandler> > _handlers;
};

#endif
