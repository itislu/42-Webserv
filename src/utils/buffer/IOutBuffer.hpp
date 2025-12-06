#pragma once
#ifndef I_OUT_BUFFER_HPP
#define I_OUT_BUFFER_HPP

#include <utils/buffer/IBuffer.hpp>

#include <cstddef>
#include <new>
#include <string>

/* ************************************************************************** */
class IOutBuffer : public virtual IBuffer
{
public:
  IOutBuffer() {}
  virtual ~IOutBuffer() {}

  // Throwing versions
  virtual void append(const std::string& data) = 0;
  virtual void append(const RawBytes& buffer, std::size_t bytes) = 0;
  virtual void removeFront(std::size_t bytes) = 0;
  virtual void replace(RawBytes& rawData) = 0;
  virtual void moveBufferToFile(const std::string& filepath) = 0;
  virtual void reset() = 0;

  // Non-throwing versions
  ExpectVoid append(const std::string& data, std::nothrow_t /*unused*/);
  ExpectVoid append(const RawBytes& buffer,
                    std::size_t bytes,
                    std::nothrow_t /*unused*/);
  ExpectVoid removeFront(std::size_t bytes, std::nothrow_t /*unused*/);
  ExpectVoid replace(RawBytes& rawData, std::nothrow_t /*unused*/);
  ExpectVoid moveBufferToFile(const std::string& filepath,
                              std::nothrow_t /*unused*/);

private:
  IOutBuffer(const IOutBuffer& other);
  IOutBuffer& operator=(const IOutBuffer& other);
};

#endif
