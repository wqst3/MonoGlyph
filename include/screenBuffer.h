#pragma once

#include "commonTypes.h"

class ScreenBuffer
{
	using Size = Point<int>;

	char* buffer_;
	Size size_;

	void allocate(int, int) noexcept;
	void deallocate() noexcept;

public:
	ScreenBuffer() noexcept;
	ScreenBuffer(int, int) noexcept;
	ScreenBuffer(Size) noexcept;

	ScreenBuffer(const ScreenBuffer&) = delete;
	ScreenBuffer& operator=(const ScreenBuffer&) = delete;

	~ScreenBuffer() noexcept;

	void resize(int, int) noexcept;
	void clear(char = ' ') noexcept;
	void flush() const noexcept;

	const char& at(int, int) const noexcept;
	char& at(int, int) noexcept;

	Size size() const noexcept { return size_; }

};

