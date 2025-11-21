#include "BufferReader.hpp"

#include <libftpp/utility.hpp>
#include <utils/IBuffer.hpp>

#include <cassert>
#include <cstddef>

/* ************************************************************************** */
// PUBLIC
void BufferReader::init(IBuffer* buffer)
{
  assert(buffer != FT_NULLPTR);
  _buffer = buffer;
  resetPosInBuff();
}

bool BufferReader::reachedEnd() const
{
  assert(_buffer != FT_NULLPTR);
  if (_posInBuff < 0) {
    return false;
  }
  return (static_cast<std::size_t>(_posInBuff) >= _buffer->size());
}

char BufferReader::getNextChar()
{
  assert(_buffer != FT_NULLPTR);
  const IBuffer::ExpectChr res = _buffer->get();
  _posInBuff++;
  return (*res);
}

long BufferReader::getPosInBuff() const
{
  return _posInBuff;
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

/* ************************************************************************** */
// PRIVATE
