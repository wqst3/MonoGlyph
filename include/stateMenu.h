#pragma once

#include "state.h"

/**
 * @brief Represents the Menu state in the MonoGlyph application.
 *
 * This state displays the main menu and responds to user input to transition
 * into other states like Infinite or Restart.
 */
class StateMenu : public State {
  /**
   * @brief Renders the menu UI.
   *
   * @param glyph Reference to the MonoGlyph instance for rendering.
   */
  void draw(MonoGlyph &glyph);

  /**
   * @brief Handles user input in the menu state.
   *
   * @param glyph Reference to the MonoGlyph instance affected by the input.
   * @param ch Character input from the user.
   */
  void input(MonoGlyph &glyph, char ch);

public:
  /**
   * @brief Called when entering the Menu state.
   *
   * Triggers a draw of the menu screen.
   *
   * @param glyph Reference to the MonoGlyph entering this state.
   */
  void onEnter(MonoGlyph &glyph) override;

  /**
   * @brief Handles the main event loop for the Menu state.
   *
   * Responds to system signals, timer events, and user input.
   *
   * @param glyph Reference to the MonoGlyph instance in this state.
   */
  void onEvent(MonoGlyph &glyph) override;

  /**
   * @brief Returns the unique identifier for this state.
   *
   * @return StateID::Menu
   */
  StateID id() const override { return StateID::Menu; }
};
