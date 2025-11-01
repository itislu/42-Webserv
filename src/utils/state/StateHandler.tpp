// IWYU pragma: private; include "StateHandler.hpp"
#pragma once
#ifndef STATE_HANDLER_HPP
#include "StateHandler.hpp"
#endif

#include <utils/state/Done.hpp>
#include <utils/state/IState.hpp>

#include <cstddef>

/* ************************************************************************** */
// PUBLIC
template<typename Context>
StateHandler<Context>::StateHandler(Context* context)
  : _context(context)
  , _state(NULL)
  , _stateHasChanged(false)
{
}

template<typename Context>
StateHandler<Context>::~StateHandler()
{
  delete _state;
}

template<typename Context>
IState<Context>* StateHandler<Context>::getState() const
{
  return _state;
}

template<typename Context>
template<typename T>
void StateHandler<Context>::setState()
{
  T* newState = new T(_context);
  delete _state;
  _state = newState;
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
  return dynamic_cast<Done<Context>*>(_state) != NULL;
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
