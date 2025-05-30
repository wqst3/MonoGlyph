#pragma once
#include "color.h"

struct Pixel {
  wchar_t ch = L' ';
  Color fg = Color::Default;
  BgColor bg = BgColor::Default;

  Pixel() = default;
  Pixel(wchar_t c) : ch(c) {}
  Pixel(wchar_t c, Color col, BgColor bgcol) : ch(c), fg(col), bg(bgcol) {}

  bool operator!=(const Pixel &other) const {
    return ch != other.ch || fg != other.fg || bg != other.bg;
  }

  operator wchar_t() const { return ch; }
};
