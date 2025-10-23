#include "BufferReader.hpp"
#include "utils/Buffer.hpp"
#include <algorithm>
#include <cstddef>
#include <iostream>

/* ************************************************************************** */
// PUBLIC
void BufferReader::init(Buffer* buffer)
{
  _buffer = buffer;
  resetPosInBuff();
}

bool BufferReader::reachedEnd() const
{
  if (_buffer == NULL) {
    return true;
  }
  return (_buffer->begin() + _posInBuff + 1) == _buffer->end();
}

char BufferReader::getNextChar()
{
  _posInBuff += 1;
  return static_cast<char>(_buffer->at(_posInBuff));
}

char BufferReader::getCurrChar() const
{
  return static_cast<char>(_buffer->at(_posInBuff));
}

long BufferReader::getPosInBuff() const
{
  return _posInBuff;
}

void BufferReader::setPosInBuff(long pos)
{
  _posInBuff = pos;
}

void BufferReader::resetPosInBuff()
{
  _posInBuff = -1;
}

void BufferReader::rewind(long bytes)
{
  if (bytes < 0) {
    return;
  }
  _posInBuff -= bytes;
}

void BufferReader::printRemaining()
{
  long istart = getPosInBuff();
  istart = std::max<long>(istart + 1, 0);
  Buffer::iterator iter = _buffer->getIterAt(istart);
  while (iter < _buffer->end() && *iter != '\r' && *iter != '\n') {
    std::cout << *iter;
    iter++;
  }
}

/* ************************************************************************** */
// PRIVATE
