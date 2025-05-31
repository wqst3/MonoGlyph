#pragma once
#include "color.h"

/// @brief Represents a single character cell with a character, foreground
/// color, and background color.
struct Pixel {
  /// @brief The character to display.
  wchar_t ch = L' ';

  /// @brief The foreground color of the character.
  Color fg = Color::Default;

  /// @brief The background color of the character.
  BgColor bg = BgColor::Default;

  /// @brief Default constructor. Initializes with a space character and default
  /// colors.
  Pixel() = default;

  /// @brief Constructs a Pixel with the given character and default colors.
  /// @param c The character to initialize with.
  Pixel(wchar_t c) : ch(c) {}

  /// @brief Constructs a Pixel with the specified character, foreground color,
  /// and background color.
  /// @param c The character to display.
  /// @param col The foreground color.
  /// @param bgcol The background color.
  Pixel(wchar_t c, Color col, BgColor bgcol) : ch(c), fg(col), bg(bgcol) {}

  /// @brief Compares two Pixels for inequality.
  /// @param other The other Pixel to compare with.
  /// @return True if any of the character, foreground color, or background
  /// color differ.
  bool operator!=(const Pixel &other) const {
    return ch != other.ch || fg != other.fg || bg != other.bg;
  }

  /// @brief Casts the Pixel to a wchar_t (returns the character).
  /// @return The character stored in the Pixel.
  operator wchar_t() const { return ch; }
};
