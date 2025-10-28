// IWYU pragma: private; include "Done.hpp"
#pragma once
#ifndef DONE_HPP
#include "Done.hpp"
#endif

#include <utils/state/IState.hpp>

template<typename Context>
Done<Context>::Done(Context* ptr)
  : IState<Context>(ptr)
{
}

template<typename Context>
Done<Context>::~Done()
{
}

template<typename Context>
void Done<Context>::run()
{
}
