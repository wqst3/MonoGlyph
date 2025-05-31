#include "drawer.h"

#include <cmath>

// === public methods ===
Drawer::Drawer(ScreenBuffer &sb) noexcept : sBuffer_(sb) {}

void Drawer::drawPixel(int x, int y, const Pixel &c) noexcept {
  Size size = sBuffer_.size();

  if (x < size.x && x >= 0 && y < size.y && y >= 0)
    if (sBuffer_.at(x, y) != c)
      sBuffer_.at(x, y) = c;
}

void Drawer::drawLine(int x1, int y1, int x2, int y2, const Pixel &c) noexcept {
  bool steep = std::abs(y2 - y1) > std::abs(x2 - x1);
  if (steep) {
    std::swap(x1, y1);
    std::swap(x2, y2);
  }

  if (x1 > x2) {
    std::swap(x1, x2);
    std::swap(y1, y2);
  }

  int dx = x2 - x1;
  int dy = std::abs(y2 - y1);
  int error = dx / 2;
  int ystep = (y1 < y2) ? 1 : -1;
  int y = y1;

  for (int x = x1; x <= x2; ++x) {
    if (steep)
      drawPixel(y, x, c);
    else
      drawPixel(x, y, c);

    error -= dy;
    if (error < 0) {
      y += ystep;
      error += dx;
    }
  }
}

void Drawer::drawString(int x, int y, const Pixel *s) noexcept {
  while (*s) {
    drawPixel(x, y, *s);
    ++s;
    ++x;
  }
}

void Drawer::drawString(int x, int y, const wchar_t *s, Color fg,
                        BgColor bg) noexcept {
  while (*s) {
    drawPixel(x, y, Pixel{*s, fg, bg});
    ++s;
    ++x;
  }
}

void Drawer::drawView(int x, int y, const std::vector<Line> &lines,
                      const Pixel &c) noexcept {
  Size size = sBuffer_.size();

  int x1, y1, x2, y2;
  for (const auto &line : lines) {
    x1 = line.x1 * size.x;
    y1 = line.y1 * size.y;
    x2 = line.x2 * size.x;
    y2 = line.y2 * size.y;

    drawLine(x1, y1, x2, y2, c);
  }
}

void Drawer::drawBuffer(int x, int y, ScreenBuffer &sb) noexcept {
  Size sbSize = sb.size();

  for (int sbY = 0; sbY < sbSize.y; ++sbY)
    for (int sbX = 0; sbX < sbSize.x; ++sbX)
      drawPixel(x + sbX, y + sbY, sb.at(sbX, sbY));
}
