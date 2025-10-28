#pragma once
#ifndef STATE_HANDLER_HPP
#define STATE_HANDLER_HPP

#include <utils/state/IState.hpp>

/* ************************************************************************** */
template<typename Context>
class StateHandler
{
public:
  explicit StateHandler(Context* context);
  ~StateHandler();

  IState<Context>* getState() const;

  template<typename T>
  void setState();

  void setDone();
  bool isDone() const;

  void setStateHasChanged(bool value);
  bool stateHasChanged() const;

private:
  StateHandler();
  StateHandler(const StateHandler& other);
  StateHandler& operator=(const StateHandler& other);

  Context* _context;
  IState<Context>* _state;
  bool _stateHasChanged;
};

#include "StateHandler.tpp" // IWYU pragma: export

#endif
