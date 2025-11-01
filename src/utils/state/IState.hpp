#pragma once
#ifndef I_STATE_HPP
#define I_STATE_HPP

/* ************************************************************************** */
template<typename Context>
class IState
{
public:
  explicit IState(Context* context);
  virtual ~IState();

  virtual void run() = 0;
  Context* getContext() const;

private:
  IState();
  IState(const IState& other);
  IState& operator=(const IState& other);

  Context* _context;
};

#include "IState.tpp" // IWYU pragma: export

#endif
