#pragma once

#include "state.h"

/**
 * @brief Represents the Infinite state of the MonoGlyph application.
 *
 * In this state, the application continuously listens to signals, timers,
 * and user input to update and render the UI in a loop without predefined end.
 */
class StateInfinite : public State {
  /**
   * @brief Draws the current UI for the Infinite state.
   *
   * @param glyph Reference to the MonoGlyph instance used for rendering.
   */
  void draw(MonoGlyph &glyph);

  /**
   * @brief Handles character input from the user.
   *
   * @param glyph Reference to the MonoGlyph instance to apply input effects on.
   * @param ch Character input from the user.
   */
  void input(MonoGlyph &glyph, char ch);

public:
  /**
   * @brief Handles the main event loop for the Infinite state.
   *
   * Sets up signal and timer file descriptors and listens to user input.
   * The loop exits when the state is changed.
   *
   * @param glyph Reference to the MonoGlyph instance this state operates on.
   */
  void onEvent(MonoGlyph &glyph) override;

  /**
   * @brief Returns the unique identifier of this state.
   *
   * @return StateID::Infinite
   */
  StateID id() const override { return StateID::Infinite; }
};
