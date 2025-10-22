#pragma once
#ifndef DONE_HPP
#define DONE_HPP

#include <cstddef>
#include <utils/state/IState.hpp>

/* ************************************************************************** */
template<typename Context>
class Done : public IState<Context>
{
public:
  explicit Done(Context* ptr);
  ~Done();

private:
  Done();
  Done(const Done& other);
  Done& operator=(const Done& other);

  void run();
};

#include "Done.tpp"

#endif // CLASS_NAME_HPP
