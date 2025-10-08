#pragma once
#ifndef I_STATE_HPP
#include "IState.hpp"
#endif

#include <cstddef>

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

template<typename Context>
IState<Context>::IState()
  : _context(NULL)
{
}

template<typename Context>
IState<Context>::IState(const IState& other)
  : _context(other._context)
{
  *this = other;
}

template<typename Context>
IState<Context>& IState<Context>::operator=(const IState& other)
{
  if (this != &other) {
    // todo copy logic
  }
  return *this;
}
