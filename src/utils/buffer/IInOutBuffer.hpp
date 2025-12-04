#pragma once
#ifndef I_IN_OUT_BUFFER_HPP
#define I_IN_OUT_BUFFER_HPP

#include <utils/buffer/IInBuffer.hpp>
#include <utils/buffer/IOutBuffer.hpp>

/* ************************************************************************** */
class IInOutBuffer
  : public IInBuffer
  , public IOutBuffer
{
public:
  IInOutBuffer() {}
  virtual ~IInOutBuffer() {}

private:
  IInOutBuffer(const IInOutBuffer& other);
  IInOutBuffer& operator=(const IInOutBuffer& other);
};

#endif
