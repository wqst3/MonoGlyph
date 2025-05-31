#pragma once

#include "state.h"

/**
 * @brief Represents the Restart state in the MonoGlyph application.
 *
 * This state allows users to restart or reinitialize certain components.
 * It waits for user input to either proceed with restarting or return to the
 * menu.
 */
class StateRestart : public State {
  /**
   * @brief Renders the restart UI.
   *
   * @param glyph Reference to the MonoGlyph instance used for rendering.
   */
  void draw(MonoGlyph &glyph);

  /**
   * @brief Handles user input during the restart state.
   *
   * @param glyph Reference to the MonoGlyph instance affected by the input.
   * @param ch Character input from the user.
   */
  void input(MonoGlyph &glyph, char ch);

public:
  /**
   * @brief Handles the main event loop for the Restart state.
   *
   * Listens to signals, timer events, and user input. Exits when the state
   * changes.
   *
   * @param glyph Reference to the MonoGlyph instance in this state.
   */
  void onEvent(MonoGlyph &glyph) override;

  /**
   * @brief Returns the unique identifier for this state.
   *
   * @return StateID::Restart
   */
  StateID id() const override { return StateID::Restart; }
};
