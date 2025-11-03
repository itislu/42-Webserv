// IWYU pragma: private; include "IState.hpp"
#pragma once
#ifndef I_STATE_HPP
#include "IState.hpp"
#endif

#include "libftpp/utility.hpp"

#include <stdexcept>

/* ************************************************************************** */
// PUBLIC

template<typename Context>
IState<Context>::IState(Context* context)
  : _context(context)
{
  if (_context == FT_NULLPTR) {
    throw std::invalid_argument("Context pointer cannot be null");
  }
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
