#pragma once

#include <random>

#include "commonTypes.h"
#include "drawer.h"
#include "fontManager.h"
#include "screenBuffer.h"
#include "signalFD.hpp"
#include "state.h"
#include "terminal.h"
#include "timerFD.hpp"

/**
 * @class MonoGlyph
 * @brief Core class managing the game state, rendering, input, and timing for a
 * glyph-based typing game.
 *
 * MonoGlyph handles terminal setup, font loading, glyph selection, and state
 * transitions. It manages three glyphs: left, main, and right, updating them as
 * the player inputs letters.
 */
class MonoGlyph {
  using Line = Vector<float>; ///< Type alias for a vector of floats
                              ///< representing a line (used in glyph segments)
  using Size = Point<int>; ///< Type alias for a 2D integer point representing
                           ///< terminal size

  bool correctLetterInput_; ///< Flag indicating if the last letter input was
                            ///< correct

  std::unique_ptr<State> currentState_; ///< Current game state

  Terminal terminal_;    ///< Terminal interface wrapper
  ScreenBuffer sBuffer_; ///< Screen buffer for offscreen rendering
  Drawer drawer_;        ///< Drawer for rendering glyphs and UI elements
  FontManager fManager_; ///< Font manager to load and access fonts

  std::random_device rd_; ///< Random device for seeding RNG
  std::mt19937 gen_;      ///< Mersenne Twister random number generator

  SignalFDHandler
      signalFDHandler_; ///< Signal file descriptor handler (e.g. for SIGWINCH)
  TimerFDHandler
      timerFDHandler_; ///< Timer file descriptor handler for timing events

  Glyph leftLetter_;  ///< Glyph displayed on the left (previous letter)
  Glyph mainLetter_;  ///< Glyph displayed in the center (current letter)
  Glyph rightLetter_; ///< Glyph displayed on the right (next letter)

public:
  /**
   * @brief Default constructor.
   *
   * Initializes terminal, screen buffer, drawer, font manager,
   * random generator, signal and timer handlers.
   * Sets terminal to raw mode and alternate buffer.
   * Loads fonts and changes to the menu state.
   */
  MonoGlyph();

  MonoGlyph(const MonoGlyph &) = delete; ///< Deleted copy constructor.
  MonoGlyph &
  operator=(const MonoGlyph &) = delete; ///< Deleted copy assignment operator.

  /**
   * @brief Destructor.
   *
   * Restores terminal to original settings and disables raw mode.
   */
  ~MonoGlyph();

  /**
   * @brief Checks if the last letter input was correct.
   * @return True if last input was correct, false otherwise.
   */
  bool correctLetterInput() const noexcept;

  /**
   * @brief Starts the main event loop.
   *
   * Runs until the current state is nullptr.
   * Catches system and general exceptions, returning error codes.
   * @return 0 on success, non-zero on error.
   */
  int start();

  /**
   * @brief Handles terminal resize event.
   *
   * Updates terminal size, resizes screen buffer, and clears the terminal.
   */
  void onResize();

  /**
   * @brief Updates the three displayed letters.
   *
   * Picks random letters from the English font and sets main and right letters.
   * Clears the left letter.
   */
  void updateLetters();

  /**
   * @brief Advances to the next letters based on input correctness.
   *
   * Moves mainLetter_ to leftLetter_, rightLetter_ to mainLetter_, and
   * selects a new random rightLetter_. Also updates correctLetterInput_ flag.
   * @param correct Whether the input was correct.
   */
  void newLetter(bool correct);

  /**
   * @brief Changes the current game state.
   *
   * Transfers ownership of the new state and calls its onEnter method.
   * @param newState New state to set.
   */
  void changeState(std::unique_ptr<State> newState);

  /**
   * @brief Gets a pointer to the current state.
   * @return Pointer to current State object or nullptr if none.
   */
  State *currentState() noexcept;

  /**
   * @brief Accessor for the terminal interface.
   * @return Reference to the Terminal object.
   */
  Terminal &terminal() noexcept;

  /**
   * @brief Accessor for the screen buffer.
   * @return Reference to the ScreenBuffer object.
   */
  ScreenBuffer &screenBuffer() noexcept;

  /**
   * @brief Accessor for the drawer.
   * @return Reference to the Drawer object.
   */
  Drawer &drawer() noexcept;

  /**
   * @brief Accessor for the font manager.
   * @return Reference to the FontManager object.
   */
  FontManager &fonts() noexcept;

  /**
   * @brief Accessor for the signal file descriptor handler.
   * @return Reference to the SignalFDHandler object.
   */
  SignalFDHandler &signalFDHandler() noexcept;

  /**
   * @brief Accessor for the timer file descriptor handler.
   * @return Reference to the TimerFDHandler object.
   */
  TimerFDHandler &timerFDHandler() noexcept;

  /**
   * @brief Accessor for the left letter glyph.
   * @return Reference to the left Glyph.
   */
  Glyph &leftLetter() noexcept;

  /**
   * @brief Accessor for the main letter glyph.
   * @return Reference to the main Glyph.
   */
  Glyph &mainLetter() noexcept;

  /**
   * @brief Accessor for the right letter glyph.
   * @return Reference to the right Glyph.
   */
  Glyph &rightLetter() noexcept;
};
