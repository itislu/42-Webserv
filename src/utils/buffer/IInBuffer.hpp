#pragma once
#ifndef I_IN_BUFFER_HPP
#define I_IN_BUFFER_HPP

#include <utils/buffer/IBuffer.hpp>

#include <cstddef>
#include <new>
#include <ostream>
#include <string>

/* ************************************************************************** */
class IInBuffer : public virtual IBuffer
{
public:
  IInBuffer() {}
  virtual ~IInBuffer() {}

  // Throwing versions
  virtual char get() = 0;
  virtual char peek() = 0;
  virtual void seek(std::size_t pos) = 0;
  virtual std::size_t pos() = 0;
  virtual std::string consumeFront(std::size_t bytes) = 0;
  virtual RawBytes consumeRawFront(std::size_t bytes) = 0;
  virtual RawBytes consumeAll() = 0;
  virtual std::string getStr(std::size_t start, std::size_t end) = 0;
  virtual RawBytes getRawBytes(std::size_t start, std::size_t end) = 0;

  // Non-throwing versions
  virtual ExpectChr get(std::nothrow_t /*unused*/) = 0;
  virtual ExpectChr peek(std::nothrow_t /*unused*/) = 0;
  virtual ExpectVoid seek(std::size_t pos, std::nothrow_t /*unused*/) = 0;
  virtual ExpectPos pos(std::nothrow_t /*unused*/) = 0;
  virtual ExpectStr consumeFront(std::size_t bytes,
                                 std::nothrow_t /*unused*/) = 0;
  virtual ExpectRaw consumeAll(std::nothrow_t /*unused*/) = 0;
  virtual ExpectStr getStr(std::size_t start,
                           std::size_t end,
                           std::nothrow_t /*unused*/) = 0;
  virtual ExpectRaw getRawBytes(std::size_t start,
                                std::size_t end,
                                std::nothrow_t /*unused*/) = 0;
  virtual ExpectRaw consumeRawFront(std::size_t bytes,
                                    std::nothrow_t /*unused*/) = 0;

private:
  IInBuffer(const IInBuffer& other);
  IInBuffer& operator=(const IInBuffer& other);
};

std::ostream& operator<<(std::ostream& out, IInBuffer& buffer);

#endif
