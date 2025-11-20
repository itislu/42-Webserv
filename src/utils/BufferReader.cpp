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
  _fail = false;
  _buffer = buffer;
  _error = IBuffer::BufferException("Success");
  resetPosInBuff();
}

bool BufferReader::reachedEnd() const
{
  assert(_buffer != FT_NULLPTR);
  if (_buffer == FT_NULLPTR) {
    return true;
  }
  if (_posInBuff < 0) {
    return false;
  }
  return (static_cast<std::size_t>(_posInBuff) >= _buffer->size());
}

char BufferReader::getNextChar()
{
  assert(_buffer != FT_NULLPTR);
  if (_fail) {
    return '\0';
  }

  const IBuffer::ExpectChr res = _buffer->get();
  if (!res.has_value()) {
    _fail = true;
    _error = res.error();
    return '\0';
  }
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
  if (_fail) {
    return;
  }
  const IBuffer::ExpectVoid res = _buffer->seek(pos);
  if (!res.has_value()) {
    _fail = true;
    _error = res.error();
    return;
  }
  _posInBuff = pos;
}

void BufferReader::resetPosInBuff()
{
  assert(_buffer != FT_NULLPTR);
  if (_fail) {
    return;
  }
  const IBuffer::ExpectVoid res = _buffer->seek(0);
  if (!res.has_value()) {
    _fail = true;
    _error = res.error();
    return;
  }
  _posInBuff = 0;
}

void BufferReader::rewind(long bytes)
{
  if (_fail || bytes <= 0) {
    return;
  }
  const long newPos = _posInBuff - bytes;
  assert(newPos >= 0);
  const IBuffer::ExpectVoid res = _buffer->seek(newPos);
  if (!res.has_value()) {
    _fail = true;
    _error = res.error();
    return;
  }
  _posInBuff = newPos;
}

bool BufferReader::fail() const
{
  return _fail;
}

const IBuffer::BufferException& BufferReader::error() const
{
  return _error;
}

/* ************************************************************************** */
// PRIVATE
