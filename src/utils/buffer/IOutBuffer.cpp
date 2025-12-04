#include "IOutBuffer.hpp"

#include <libftpp/expected.hpp>
#include <utils/buffer/IBuffer.hpp>

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <new>
#include <string>

/* ************************************************************************** */
// PUBLIC

// Non-throwing versions

IBuffer::ExpectVoid IOutBuffer::append(const std::string& data,
                                       std::nothrow_t /*unused*/)
{
  try {
    append(data);
    return ExpectVoid();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectVoid IOutBuffer::append(const IOutBuffer::RawBytes& buffer,
                                       std::size_t bytes,
                                       std::nothrow_t /*unused*/)
{
  try {
    append(buffer, bytes);
    return ExpectVoid();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectVoid IOutBuffer::removeFront(std::size_t bytes,
                                            std::nothrow_t /*unused*/)
{
  try {
    removeFront(bytes);
    return ExpectVoid();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectVoid IOutBuffer::replace(RawBytes& rawData,
                                        std::nothrow_t /*unused*/)
{
  try {
    replace(rawData);
    return ExpectVoid();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

IBuffer::ExpectVoid IOutBuffer::moveBufferToFile(const std::string& filepath,
                                                 std::nothrow_t /*unused*/)
{
  try {
    moveBufferToFile(filepath);
    return ExpectVoid();
  } catch (const std::exception& e) {
    return ft::unexpected<BufferException>(e);
  }
}

/* ************************************************************************** */
// PRIVATE
