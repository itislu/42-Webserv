#pragma once
#ifndef PIPE_HPP
#define PIPE_HPP

#include <exception>

/* ************************************************************************** */
class Pipe
{
public:
  Pipe();
  ~Pipe();
  class PipeException;

  void init();
  void close();
  void closeRead();
  void closeWrite();

  int getReadFd() const;
  int getWriteFd() const;

private:
  Pipe(const Pipe& other);
  Pipe& operator=(const Pipe& other);

  static void _setFdNonBlocking(int fdes);

  int _pipefd[2];
};

/* ************************************************************************** */
// Exception
class Pipe::PipeException : public std::exception
{
public:
  const char* what() const throw();
};

#endif
