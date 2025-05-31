#include "screenBuffer.h"

#include <cassert>
#include <cstring>
#include <iostream>

// === public methods ===
ScreenBuffer::ScreenBuffer() noexcept
    : buffer_(nullptr), prevBuffer_(nullptr), size_{0, 0} {
  resize(0, 0);
}

ScreenBuffer::ScreenBuffer(int cols, int rows) noexcept
    : buffer_(nullptr), prevBuffer_(nullptr), size_{0, 0} {
  resize(cols, rows);
}

ScreenBuffer::ScreenBuffer(Size size) noexcept : ScreenBuffer(size.x, size.y) {}

ScreenBuffer::~ScreenBuffer() noexcept { deallocate(); }

void ScreenBuffer::syncBuffers() const noexcept {
  std::memcpy(prevBuffer_, buffer_, sizeof(Pixel) * size_.x * size_.y);
}

void ScreenBuffer::resize(int cols, int rows) noexcept {
  if (cols == size_.x && rows == size_.y)
    return;

  deallocate();
  size_ = {cols, rows};

  if (cols > 0 && rows > 0)
    allocate(cols, rows);

  clear();
}

void ScreenBuffer::clear(Pixel fillChar) noexcept {
  if (buffer_)
    std::fill_n(buffer_, size_.x * size_.y, fillChar);
}

void ScreenBuffer::flush() const noexcept {
  if (!buffer_ || !prevBuffer_)
    return;

  for (int y = 0; y < size_.y; ++y) {
    for (int x = 0; x < size_.x; ++x) {
      int i = y * size_.x + x;

      const Pixel &curr = buffer_[i];
      const Pixel &prev = prevBuffer_[i];

      if (curr != prev) {
        std::wcout << L"\033[" << (y + 1) << L";" << (x + 1) << L"H";

        std::wcout << L"\033[" << static_cast<int>(curr.fg) << L";"
                   << static_cast<int>(curr.bg) << L"m";

        std::wcout << curr.ch;

        std::wcout << L"\033[0m";
      }
    }
  }
  syncBuffers();
  std::wcout.flush();
}

const Pixel &ScreenBuffer::at(int x, int y) const noexcept {
  return buffer_[y * size_.x + x];
}

Pixel &ScreenBuffer::at(int x, int y) noexcept {
  return buffer_[y * size_.x + x];
}

// === private methods ===
void ScreenBuffer::allocate(int cols, int rows) noexcept {
  buffer_ = new Pixel[cols * rows];
  prevBuffer_ = new Pixel[cols * rows];
}

void ScreenBuffer::deallocate() noexcept {
  delete[] buffer_;
  delete[] prevBuffer_;
  buffer_ = nullptr;
  prevBuffer_ = nullptr;
}
