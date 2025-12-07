#include "BufferReader.hpp"

#include <libftpp/expected.hpp>
#include <libftpp/utility.hpp>
#include <utils/buffer/IBuffer.hpp>
#include <utils/buffer/IInBuffer.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <exception>
#include <new>

/* ************************************************************************** */
// PUBLIC

void BufferReader::init(IInBuffer* buffer)
{
  assert(buffer != FT_NULLPTR);
  _buffer = buffer;
  resetPosInBuff();
}

// Throwing versions

char BufferReader::getNextChar()
{
  assert(_buffer != FT_NULLPTR);
  const char chr = _buffer->get();
  _posInBuff++;
  return chr;
}

void BufferReader::setPosInBuff(std::size_t pos)
{
  assert(_buffer != FT_NULLPTR);
  _buffer->seek(pos);
  _posInBuff = pos;
}

void BufferReader::resetPosInBuff()
{
  assert(_buffer != FT_NULLPTR);
  _buffer->seek(0);
  _posInBuff = 0;
}

void BufferReader::rewind(std::size_t bytes)
{
  if (bytes == 0) {
    return;
  }
  bytes = std::min(bytes, _posInBuff);
  setPosInBuff(_posInBuff - bytes);
}

// Non-throwing versions

BufferReader::ExpectChr BufferReader::getNextChar(std::nothrow_t /*unused*/)
{
  try {
    return getNextChar();
  } catch (const std::exception& e) {
    return ft::unexpected<IBuffer::BufferException>(e);
  }
}

BufferReader::ExpectVoid BufferReader::setPosInBuff(std::size_t pos,
                                                    std::nothrow_t /*unused*/)
{
  try {
    setPosInBuff(pos);
    return ExpectVoid();
  } catch (const std::exception& e) {
    return ft::unexpected<IBuffer::BufferException>(e);
  }
}

BufferReader::ExpectVoid BufferReader::resetPosInBuff(std::nothrow_t /*unused*/)
{
  try {
    resetPosInBuff();
    return ExpectVoid();
  } catch (const std::exception& e) {
    return ft::unexpected<IBuffer::BufferException>(e);
  }
}

BufferReader::ExpectVoid BufferReader::rewind(std::size_t bytes,
                                              std::nothrow_t /*unused*/)
{
  try {
    rewind(bytes);
    return ExpectVoid();
  } catch (const std::exception& e) {
    return ft::unexpected<IBuffer::BufferException>(e);
  }
}

bool BufferReader::reachedEnd() const
{
  assert(_buffer != FT_NULLPTR);
  return (_posInBuff >= _buffer->size());
}

std::size_t BufferReader::getPosInBuff() const
{
  return _posInBuff;
}

/* ************************************************************************** */
// PRIVATE
