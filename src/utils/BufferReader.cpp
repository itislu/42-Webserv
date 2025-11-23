#include "BufferReader.hpp"

#include <libftpp/expected.hpp>
#include <libftpp/utility.hpp>
#include <utils/IBuffer.hpp>

#include <cassert>
#include <cstddef>
#include <new>

/* ************************************************************************** */
// PUBLIC

void BufferReader::init(IBuffer* buffer)
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

void BufferReader::setPosInBuff(long pos)
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

void BufferReader::rewind(long bytes)
{
  if (bytes <= 0) {
    return;
  }
  const long newPos = _posInBuff - bytes;
  assert(newPos >= 0);
  setPosInBuff(newPos);
}

// Non-throwing versions

BufferReader::ExpectChr BufferReader::getNextChar(std::nothrow_t /*unused*/)
{
  try {
    return getNextChar();
  } catch (const IBuffer::BufferException& e) {
    return ft::unexpected<IBuffer::BufferException>(e);
  }
}

BufferReader::ExpectVoid BufferReader::setPosInBuff(long pos,
                                                    std::nothrow_t /*unused*/)
{
  try {
    setPosInBuff(pos);
    return ExpectVoid();
  } catch (const IBuffer::BufferException& e) {
    return ft::unexpected<IBuffer::BufferException>(e);
  }
}

BufferReader::ExpectVoid BufferReader::resetPosInBuff(std::nothrow_t /*unused*/)
{
  try {
    resetPosInBuff();
    return ExpectVoid();
  } catch (const IBuffer::BufferException& e) {
    return ft::unexpected<IBuffer::BufferException>(e);
  }
}

BufferReader::ExpectVoid BufferReader::rewind(long bytes,
                                              std::nothrow_t /*unused*/)
{
  try {
    rewind(bytes);
    return ExpectVoid();
  } catch (const IBuffer::BufferException& e) {
    return ft::unexpected<IBuffer::BufferException>(e);
  }
}

bool BufferReader::reachedEnd() const
{
  assert(_buffer != FT_NULLPTR);
  if (_posInBuff < 0) {
    return false;
  }
  return (static_cast<std::size_t>(_posInBuff) >= _buffer->size());
}

long BufferReader::getPosInBuff() const
{
  return _posInBuff;
}

/* ************************************************************************** */
// PRIVATE
