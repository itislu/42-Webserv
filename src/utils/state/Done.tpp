#pragma once
#ifndef DONE_HPP
#include "utils/state/Done.hpp"
#endif

#include <utils/state/IState.hpp>


template<typename Context>
Done<Context>::Done(Context* ptr)
  : IState<Context>(ptr)
{
}

template<typename Context>
Done<Context>::~Done() {}

template<typename Context>
void Done<Context>::run() {}
