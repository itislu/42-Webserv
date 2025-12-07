#include "BaseHeaderValidator.hpp"

#include <cstddef>
#include <iostream>
#include <string>

/* ************************************************************************** */
// INIT

const std::size_t BaseHeaderValidator::_defaultMaxFieldLineSize;
const std::size_t BaseHeaderValidator::_defaultMaxHeaderSize;

/* ************************************************************************** */
// PUBLIC

BaseHeaderValidator::BaseHeaderValidator()
  : _maxFieldLineSize(_defaultMaxFieldLineSize)
  , _maxHeaderSize(_defaultMaxHeaderSize)
  , _totalSizeHeaders(0)
{
}

bool BaseHeaderValidator::isValid(const std::string& name,
                                  const std::string& value)
{
  (void)name;
  (void)value;
  return true;
}

bool BaseHeaderValidator::fieldLineTooLarge(std::size_t newBytes) const
{
  return newBytes > _maxFieldLineSize;
}

bool BaseHeaderValidator::headerTooLarge(std::size_t newBytes)
{
  if (_totalSizeHeaders + newBytes > _maxHeaderSize) {
    return true;
  }
  _totalSizeHeaders += newBytes;
  return false;
}

void BaseHeaderValidator::setMaxFieldLineSize(std::size_t value)
{
  _maxFieldLineSize = value;
}

void BaseHeaderValidator::setMaxHeaderSize(std::size_t value)
{
  _maxHeaderSize = value;
}

void BaseHeaderValidator::resetMaxFieldLineSize()
{
  _maxFieldLineSize = _defaultMaxFieldLineSize;
}

void BaseHeaderValidator::resetMaxHeaderSize()
{
  _maxHeaderSize = _defaultMaxHeaderSize;
}

/* ************************************************************************** */
// PRIVATE
