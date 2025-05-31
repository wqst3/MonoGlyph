#pragma once

#include <cstddef>
#include <termios.h>

#include "commonTypes.h"

/**
 * @class Terminal
 * @brief A utility class for managing terminal behavior such as raw mode,
 * buffer control, cursor visibility, and screen clearing.
 */
class Terminal {
  using Size = Point<int>;

  termios orig_termios_; ///< Original terminal attributes before raw mode
  termios raw_;          ///< Terminal attributes configured for raw mode

  Size size_; ///< Current size of the terminal

  /**
   * @brief Safely writes data to a file descriptor.
   * @param fd File descriptor to write to.
   * @param data Data to write.
   * @param len Length of the data.
   * @param desc Description of the write operation (used in error messages).
   * @throws std::runtime_error if the write operation fails.
   */
  void safeWrite(int fd, const char *data, size_t len, const char *desc);

public:
  /**
   * @brief Constructs a Terminal object and initializes the terminal size.
   */
  Terminal();

  /**
   * @brief Updates and returns the current size of the terminal.
   * @return The current terminal size as a Point<int> (columns x, rows y).
   * @throws std::system_error if the terminal size cannot be retrieved.
   */
  Size updateSize();

  /**
   * @brief Enables raw mode for terminal input.
   * @throws std::system_error if terminal attributes cannot be read or set.
   */
  void enableRawMode();

  /**
   * @brief Disables raw mode and restores original terminal settings.
   */
  void disableRawMode();

  /**
   * @brief Switches to the alternate screen buffer.
   */
  void altBuffer();

  /**
   * @brief Hides the terminal cursor.
   */
  void hideCur();

  /**
   * @brief Clears the terminal screen and moves the cursor to the home
   * position.
   */
  void clear();

  /**
   * @brief Restores the terminal to its original buffer and shows the cursor.
   */
  void restore();

  /**
   * @brief Gets the cached size of the terminal.
   * @return Terminal size as a Point<int>.
   */
  Size size() const noexcept { return size_; }
};
