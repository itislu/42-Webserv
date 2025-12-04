#pragma once
#ifndef I_BUFFER_HPP
#define I_BUFFER_HPP

#include <libftpp/expected.hpp>

#include <cstddef>
#include <exception>
#include <string>
#include <sys/types.h>
#include <vector>

/* ************************************************************************** */
class IBuffer
{
public:
  class BufferException;

  typedef std::vector<char> RawBytes;
  typedef ft::expected<void, BufferException> ExpectVoid;
  typedef ft::expected<char, BufferException> ExpectChr;
  typedef ft::expected<std::string, BufferException> ExpectStr;
  typedef ft::expected<RawBytes, BufferException> ExpectRaw;
  typedef ft::expected<std::size_t, BufferException> ExpectPos;

  IBuffer() {}
  virtual ~IBuffer() {}

  virtual bool isEmpty() const = 0;
  virtual std::size_t size() const = 0;
  virtual ssize_t send(int fdes, std::size_t bytes) = 0;

private:
  IBuffer(const IBuffer& other);
  IBuffer& operator=(const IBuffer& other);
};

/* ************************************************************************** */
// Exceptions
class IBuffer::BufferException : public std::exception
{
public:
  explicit BufferException(const char* message);
  explicit BufferException(const std::exception& exception);
  ~BufferException() throw() {}
  BufferException(const BufferException& other) throw();
  BufferException& operator=(const BufferException& other) throw();
  const char* what() const throw();

private:
  const char* _message;
};

#endif
