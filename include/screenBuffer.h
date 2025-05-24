#pragma once

#include "commonTypes.h"

class ScreenBuffer
{
	using Size = Point<int>;

	wchar_t* buffer_;
	wchar_t* prevBuffer_;
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
	void clear(wchar_t = L' ') noexcept;
	void flush() const noexcept;

	const wchar_t& at(int, int) const noexcept;
	wchar_t& at(int, int) noexcept;

	Size size() const noexcept { return size_; }

};

