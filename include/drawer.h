#pragma once

#include <vector>

#include "commonTypes.h"
#include "screenBuffer.h"

/**
 * @class Drawer
 * @brief A utility class for drawing pixels, lines, strings, and buffers to a
 * ScreenBuffer.
 */
class Drawer {
  using Line = Vector<float>;
  using Size = Point<int>;
  ScreenBuffer &sBuffer_;

public:
  /**
   * @brief Constructs a Drawer object.
   * @param buffer Reference to a ScreenBuffer to draw on.
   */
  Drawer(ScreenBuffer &buffer) noexcept;

  /**
   * @brief Draws a single pixel at the specified coordinates.
   * @param x X-coordinate.
   * @param y Y-coordinate.
   * @param c Pixel color and attributes.
   */
  void drawPixel(int x, int y, const Pixel &c) noexcept;

  /**
   * @brief Draws a line between two points using a specified pixel.
   * @param x1 Starting X-coordinate.
   * @param y1 Starting Y-coordinate.
   * @param x2 Ending X-coordinate.
   * @param y2 Ending Y-coordinate.
   * @param c Pixel to use for the line.
   */
  void drawLine(int x1, int y1, int x2, int y2, const Pixel &c) noexcept;

  /**
   * @brief Draws a dynamic letter shape at a given position.
   * @param x X-coordinate.
   * @param y Y-coordinate.
   * @param c Pixel to use.
   * @return The size of the drawn letter.
   */
  Size drawDynamicLetter(int x, int y, const Pixel &c) noexcept;

  /**
   * @brief Draws a null-terminated array of Pixel objects as a string.
   * @param x Starting X-coordinate.
   * @param y Starting Y-coordinate.
   * @param s Null-terminated array of Pixels.
   */
  void drawString(int x, int y, const Pixel *s) noexcept;

  /**
   * @brief Draws a wide-character string using foreground and background
   * colors.
   * @param x Starting X-coordinate.
   * @param y Starting Y-coordinate.
   * @param s Null-terminated wide string.
   * @param fg Foreground color. Defaults to Color::Default.
   * @param bg Background color. Defaults to BgColor::Default.
   */
  void drawString(int x, int y, const wchar_t *s, Color fg = Color::Default,
                  BgColor bg = BgColor::Default) noexcept;

  /**
   * @brief Draws a 2D view from a vector of lines using the specified pixel.
   * @param x X-offset.
   * @param y Y-offset.
   * @param lines 2D data to draw.
   * @param c Pixel to use for drawing.
   */
  void drawView(int x, int y, const std::vector<Line> &lines,
                const Pixel &c) noexcept;

  /**
   * @brief Draws another ScreenBuffer at the specified location.
   * @param x X-offset.
   * @param y Y-offset.
   * @param sb ScreenBuffer to draw.
   */
  void drawBuffer(int x, int y, ScreenBuffer &sb) noexcept;
};
