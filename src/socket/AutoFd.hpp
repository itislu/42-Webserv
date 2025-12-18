#ifndef AUTOFD_HPP
#define AUTOFD_HPP

#include <libftpp/movable.hpp>

#include <set>

class AutoFdSubscriber
{
public:
  AutoFdSubscriber() {}
  virtual ~AutoFdSubscriber() {}

  virtual void onClose(int fdes) = 0;

private:
  AutoFdSubscriber(const AutoFdSubscriber&);
  AutoFdSubscriber& operator=(const AutoFdSubscriber&);
};

class AutoFd
{
public:
  explicit AutoFd(int fdes);
  // NOLINTNEXTLINE(google-explicit-constructor): Move constructor.
  AutoFd(ft::rvalue<AutoFd>& other);
  AutoFd& operator=(ft::rvalue<AutoFd>& other);
  ~AutoFd();

  int get() const;
  void set(int fdes);

  void subscribe(AutoFdSubscriber& subscriber);
  void unsubscribe(AutoFdSubscriber& subscriber);

private:
  FT_MOVABLE_BUT_NOT_COPYABLE(AutoFd)

  void _close();
  void _notifyClose();

  int _fd;
  std::set<AutoFdSubscriber*> _subscribers;
};

#endif
