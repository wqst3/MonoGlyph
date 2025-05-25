#pragma once

#include "commonTypes.h"
#include "pixel.h"

class ScreenBuffer
{
	using Size = Point<int>;

	Pixel* buffer_;
	Pixel* prevBuffer_;
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

	void syncBuffers() const noexcept;

	void resize(int, int) noexcept;
	void clear(Pixel = L' ') noexcept;
	void flush() const noexcept;

	const Pixel& at(int, int) const noexcept;
	Pixel& at(int, int) noexcept;

	Size size() const noexcept { return size_; }

};

