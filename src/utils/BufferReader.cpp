#include "BufferReader.hpp"

#include <utils/IBuffer.hpp>

#include <cassert>
#include <cstddef>

/* ************************************************************************** */
// PUBLIC

BufferReader::BufferReader(IBuffer& buffer)
  : _buffer(&buffer)
  , _posInBuff(0)
{
  resetPosInBuff();
}

bool BufferReader::reachedEnd() const
{
  if (_posInBuff < 0) {
    return false;
  }
  return (static_cast<std::size_t>(_posInBuff) >= _buffer->size());
}

char BufferReader::getNextChar()
{
  const IBuffer::ExpectChr res = _buffer->get();
  _posInBuff++;
  return *res;
}

long BufferReader::getPosInBuff() const
{
  return _posInBuff;
}

void BufferReader::setPosInBuff(long pos)
{
  _buffer->seek(pos);
  _posInBuff = pos;
}

void BufferReader::resetPosInBuff()
{
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
