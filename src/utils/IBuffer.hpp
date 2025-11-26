#pragma once
#ifndef I_BUFFER_HPP
#define I_BUFFER_HPP

#include <libftpp/expected.hpp>

#include <cstddef>
#include <exception>
#include <new>
#include <ostream>
#include <string>
#include <vector>

/* **************************************************************************
 */
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

  // Throwing versions
  virtual char get() = 0;
  virtual char peek() = 0;
  virtual void seek(std::size_t pos) = 0;
  virtual std::size_t pos() = 0;
  virtual void append(const std::string& data) = 0;
  virtual void append(const RawBytes& buffer, std::size_t bytes) = 0;
  virtual void removeFront(std::size_t bytes) = 0;
  virtual std::string consumeFront(std::size_t bytes) = 0;
  virtual RawBytes consumeRawFront(std::size_t bytes) = 0;
  virtual RawBytes consumeAll() = 0;
  virtual std::string getStr(std::size_t start, std::size_t end) = 0;
  virtual RawBytes getRawBytes(std::size_t start, std::size_t end) = 0;
  virtual void replace(RawBytes& rawData) = 0;
  virtual void moveBufferToFile(const std::string& filepath) = 0;

  // Non-throwing versions
  virtual ExpectChr get(std::nothrow_t /*unused*/) = 0;
  virtual ExpectChr peek(std::nothrow_t /*unused*/) = 0;
  virtual ExpectVoid seek(std::size_t pos, std::nothrow_t /*unused*/) = 0;
  virtual ExpectPos pos(std::nothrow_t /*unused*/) = 0;
  virtual ExpectVoid append(const std::string& data,
                            std::nothrow_t /*unused*/) = 0;
  virtual ExpectVoid append(const RawBytes& buffer,
                            std::size_t bytes,
                            std::nothrow_t /*unused*/) = 0;
  virtual ExpectVoid removeFront(std::size_t bytes,
                                 std::nothrow_t /*unused*/) = 0;
  virtual ExpectStr consumeFront(std::size_t bytes,
                                 std::nothrow_t /*unused*/) = 0;
  virtual ExpectRaw consumeRawFront(std::size_t bytes,
                                    std::nothrow_t /*unused*/) = 0;
  virtual ExpectRaw consumeAll(std::nothrow_t /*unused*/) = 0;
  virtual ExpectStr getStr(std::size_t start,
                           std::size_t end,
                           std::nothrow_t /*unused*/) = 0;
  virtual ExpectRaw getRawBytes(std::size_t start,
                                std::size_t end,
                                std::nothrow_t /*unused*/) = 0;
  virtual ExpectVoid replace(RawBytes& rawData, std::nothrow_t /*unused*/) = 0;
  virtual ExpectVoid moveBufferToFile(const std::string& filepath,
                                      std::nothrow_t /*unused*/) = 0;

  virtual bool isEmpty() const = 0;
  virtual std::size_t size() const = 0;

private:
  IBuffer(const IBuffer& other);
  IBuffer& operator=(const IBuffer& other);
};

std::ostream& operator<<(std::ostream& out, IBuffer& buffer);

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
