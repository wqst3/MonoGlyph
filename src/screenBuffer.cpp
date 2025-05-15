#include "screenBuffer.h"

#include <iostream>
#include <cassert>
#include <cstring>

// === public methods ===
ScreenBuffer::ScreenBuffer() noexcept
  : buffer_(nullptr), size_{0, 0}
{
	resize(0, 0);
}

ScreenBuffer::ScreenBuffer(int cols, int rows) noexcept
  : buffer_(nullptr), size_{0, 0}
{
	resize(cols, rows);
}

ScreenBuffer::ScreenBuffer(Size size) noexcept
  : ScreenBuffer(size.x, size.y) {}

ScreenBuffer::~ScreenBuffer() noexcept
{
	deallocate();
}


void ScreenBuffer::resize(int cols, int rows) noexcept
{
	if (cols == size_.x && rows == size_.y)
		return;

	deallocate();
	size_ = {cols, rows};

	if (cols > 0 && rows > 0)
		allocate(cols, rows);

	clear();
}

void ScreenBuffer::clear(char fillChar) noexcept
{
	if (buffer_)
		std::memset(buffer_, fillChar, size_.x * size_.y);
}

void ScreenBuffer::flush() const noexcept
{
	if (!buffer_) return;

	std::cout.write(buffer_, static_cast<std::streamsize>(size_.x * size_.y));

	std::cout.flush();
}


const char& ScreenBuffer::at(int x, int y) const noexcept
{
	return buffer_[y * size_.x + x];
}

char& ScreenBuffer::at(int x, int y) noexcept
{
	return buffer_[y * size_.x + x];
}


// === private methods ===
void ScreenBuffer::allocate(int cols, int rows) noexcept
{
	buffer_ = new char[cols * rows];
}

void ScreenBuffer::deallocate() noexcept
{
	delete [] buffer_;
	buffer_ = nullptr;
}

