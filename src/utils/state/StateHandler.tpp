// IWYU pragma: private; include "StateHandler.hpp"
#pragma once
#ifndef STATE_HANDLER_HPP
#include "StateHandler.hpp"
#endif

#include <libftpp/memory.hpp>
#include <libftpp/utility.hpp>
#include <utils/state/Done.hpp>
#include <utils/state/IState.hpp>

/* ************************************************************************** */
// PUBLIC
template<typename Context>
StateHandler<Context>::StateHandler(Context* context)
  : _context(context)
  , _stateHasChanged(false)
{
}

template<typename Context>
const ft::unique_ptr<IState<Context> >& StateHandler<Context>::getState() const
{
  return _state;
}

template<typename Context>
template<typename T>
void StateHandler<Context>::setState()
{
  _state.reset(new T(_context));
  setStateHasChanged(true);
}

template<typename Context>
void StateHandler<Context>::setDone()
{
  setState<Done<Context> >();
}

template<typename Context>
bool StateHandler<Context>::isDone() const
{
  return dynamic_cast<Done<Context>*>(_state.get()) != FT_NULLPTR;
}

template<typename Context>
void StateHandler<Context>::setStateHasChanged(bool value)
{
  _stateHasChanged = value;
}

template<typename Context>
bool StateHandler<Context>::stateHasChanged() const
{
  return _stateHasChanged;
}

/* ************************************************************************** */
// PRIVATE
