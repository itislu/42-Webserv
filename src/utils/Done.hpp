#pragma once
#ifndef DONE_HPP
#define DONE_HPP

#include <cstddef>
#include <utils/IState.hpp>

/* ************************************************************************** */
template<typename Context>
class Done : public IState<Context>
{
public:
  explicit Done(Context* ptr);
  ~Done();

  void run();
};

#include "Done.tpp"

#endif // CLASS_NAME_HPP
