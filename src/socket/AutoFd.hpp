#ifndef AUTOFD_HPP
#define AUTOFD_HPP

class AutoFd
{
public:
  explicit AutoFd(int fdes);
  ~AutoFd();

  int get() const;
  void set(int fdes);

private:
  AutoFd(const AutoFd& other);
  AutoFd& operator=(const AutoFd& other);
  int _fd;
};

#endif
