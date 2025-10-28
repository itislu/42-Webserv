// IWYU pragma: private; include "IState.hpp"
#pragma once
#ifndef I_STATE_HPP
#include "IState.hpp"
#endif

/* ************************************************************************** */
// PUBLIC

template<typename Context>
IState<Context>::IState(Context* context)
  : _context(context)
{
}

template<typename Context>
IState<Context>::~IState()
{
}

template<typename Context>
Context* IState<Context>::getContext() const
{
  return _context;
}

/* ************************************************************************** */
// PRIVATE
