#pragma once

#include "MonoGlyph.h"

/**
 * @class InterfaceDrawer
 * @brief Handles drawing the user interface for the MonoGlyph application.
 *
 * This class is responsible for rendering the menu, buttons, and main/side
 * letters on the terminal screen using the MonoGlyph's drawer.
 */
class InterfaceDrawer {
  /// Alias for a line of floats
  using Line = Vector<float>;

  /// Alias for a 2D size with integer coordinates
  using Size = Point<int>;

  /// Reference to the main MonoGlyph object used for drawing and state
  MonoGlyph &MonoGlyph_;

public:
  /**
   * @brief Constructs an InterfaceDrawer.
   * @param MonoGlyph Reference to the MonoGlyph instance to use for drawing.
   */
  InterfaceDrawer(MonoGlyph &MonoGlyph);

  /**
   * @brief Draws the main menu interface.
   */
  void menu();

  /**
   * @brief Draws the upper menu bar including title and mode indicators.
   */
  void upMenu();

  /**
   * @brief Draws the restart button at the bottom of the screen.
   */
  void restartButton();

  /**
   * @brief Draws the main letter in the center of the screen.
   */
  void mainLetter();

  /**
   * @brief Draws the letter to the left of the main letter.
   */
  void leftLetter();

  /**
   * @brief Draws the letter to the right of the main letter.
   */
  void rightLetter();
};
