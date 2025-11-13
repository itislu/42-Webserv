#pragma once
#ifndef I_BUFFER_HPP
#define I_BUFFER_HPP

#include <libftpp/expected.hpp>

#include <bits/c++config.h>
#include <cstdio>
#include <exception>
#include <string>
#include <vector>

/* ************************************************************************** */
class IBuffer
{
public:
  IBuffer();
  virtual ~IBuffer();

  class BufferException;

  typedef std::vector<char> Container;
  typedef ft::expected<void, BufferException> ExpectVoid;
  typedef ft::expected<char, BufferException> ExpectChr;
  typedef ft::expected<std::string, BufferException> ExpectStr;

  virtual ExpectChr get() = 0;
  virtual ExpectChr peek() = 0;
  virtual ExpectVoid seek(std::size_t pos) = 0;
  virtual ExpectVoid append(const std::string& data) = 0;
  virtual ExpectVoid append(const Container& buffer, long bytes) = 0;
  virtual ExpectVoid removeFront(std::size_t bytes) = 0;
  virtual ExpectStr consumFront(std::size_t bytes) = 0;
  virtual std::size_t size() const = 0;

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
  ~BufferException() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {};
  BufferException(const BufferException& other);
  BufferException& operator=(const BufferException& other);
  const char* what() const throw();

private:
  const char* _message;
};

#endif
