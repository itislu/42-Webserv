#pragma once
#ifndef BASE_HEADER_VALIDATOR_HPP
#define BASE_HEADER_VALIDATOR_HPP

#include <cstddef>
#include <string>

/* ************************************************************************** */
class BaseHeaderValidator
{
public:
  BaseHeaderValidator();
  virtual ~BaseHeaderValidator() {}

  virtual bool isValid(const std::string& name, const std::string& value);
  virtual bool fieldLineTooLarge(std::size_t newBytes) const;
  virtual bool headerTooLarge(std::size_t newBytes);

  void setMaxFieldLineSize(std::size_t value);
  void setMaxHeaderSize(std::size_t value);
  void resetMaxFieldLineSize();
  void resetMaxHeaderSize();

private:
  BaseHeaderValidator(const BaseHeaderValidator& other);
  BaseHeaderValidator& operator=(const BaseHeaderValidator& other);

  static const std::size_t _defaultMaxFieldLineSize = 8192;
  static const std::size_t _defaultMaxHeaderSize = 8192;
  std::size_t _maxFieldLineSize;
  std::size_t _maxHeaderSize;

  std::size_t _totalSizeHeaders;
};

#endif
