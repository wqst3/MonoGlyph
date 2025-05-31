#pragma once

#include "commonTypes.h"
#include "pixel.h"

/**
 * @class ScreenBuffer
 * @brief Represents a 2D screen buffer for rendering characters with
 * attributes.
 */
class ScreenBuffer {
  using Size = Point<int>;

  Pixel *buffer_; ///< Pointer to the current frame buffer.
  Pixel *
      prevBuffer_; ///< Pointer to the previous frame buffer (used for diffing).
  Size size_;      ///< Dimensions of the buffer (columns x rows).

  /**
   * @brief Allocates memory for the buffers.
   * @param cols Number of columns.
   * @param rows Number of rows.
   */
  void allocate(int cols, int rows) noexcept;

  /**
   * @brief Deallocates the memory for the buffers.
   */
  void deallocate() noexcept;

public:
  /**
   * @brief Default constructor, creates an empty screen buffer.
   */
  ScreenBuffer() noexcept;

  /**
   * @brief Constructs a screen buffer with the specified number of columns and
   * rows.
   * @param cols Number of columns.
   * @param rows Number of rows.
   */
  ScreenBuffer(int cols, int rows) noexcept;

  /**
   * @brief Constructs a screen buffer using a Size object.
   * @param size Size of the buffer (columns x rows).
   */
  ScreenBuffer(Size size) noexcept;

  ScreenBuffer(const ScreenBuffer &) = delete; /**< Copying is disabled. */
  ScreenBuffer &
  operator=(const ScreenBuffer &) = delete; /**< Assignment is disabled. */

  /**
   * @brief Destructor that frees buffer memory.
   */
  ~ScreenBuffer() noexcept;

  /**
   * @brief Copies the current buffer to the previous buffer.
   */
  void syncBuffers() const noexcept;

  /**
   * @brief Resizes the buffer to the new dimensions.
   * @param cols New number of columns.
   * @param rows New number of rows.
   */
  void resize(int cols, int rows) noexcept;

  /**
   * @brief Clears the screen buffer with a specified character.
   * @param fillChar The character to fill the buffer with. Defaults to space.
   */
  void clear(Pixel fillChar = L' ') noexcept;

  /**
   * @brief Outputs only the changed characters to the console.
   */
  void flush() const noexcept;

  /**
   * @brief Provides read-only access to a pixel at given coordinates.
   * @param x X coordinate (column).
   * @param y Y coordinate (row).
   * @return Reference to the pixel at the specified location.
   */
  const Pixel &at(int x, int y) const noexcept;

  /**
   * @brief Provides writable access to a pixel at given coordinates.
   * @param x X coordinate (column).
   * @param y Y coordinate (row).
   * @return Reference to the pixel at the specified location.
   */
  Pixel &at(int x, int y) noexcept;

  /**
   * @brief Returns the size of the buffer.
   * @return Size of the buffer (columns x rows).
   */
  Size size() const noexcept { return size_; }
};
