#pragma once

#include <memory>

#include "stateID.h"

class MonoGlyph;

/**
 * @brief Abstract base class representing a state in a state machine.
 *
 * This class defines the interface for different states that a MonoGlyph
 * can be in. Each state must handle events and provide its unique identifier.
 */
class State {
public:
  /**
   * @brief Virtual destructor to ensure proper cleanup of derived classes.
   */
  virtual ~State() = default;

  /**
   * @brief Called when the state is entered.
   *
   * Override this method to implement behavior that should occur when the
   * MonoGlyph enters this state.
   *
   * @param glyph Reference to the MonoGlyph entering this state.
   */
  virtual void onEnter(MonoGlyph &glyph) {}

  /**
   * @brief Handle an event while in this state.
   *
   * This method must be implemented by derived classes to define how the
   * MonoGlyph should respond to events while in this state.
   *
   * @param glyph Reference to the MonoGlyph handling the event.
   */
  virtual void onEvent(MonoGlyph &glyph) = 0;

  /**
   * @brief Get the unique identifier of the state.
   *
   * This method must be implemented by derived classes to return the state's
   * unique identifier.
   *
   * @return StateID representing the identifier of the state.
   */
  virtual StateID id() const = 0;
};
