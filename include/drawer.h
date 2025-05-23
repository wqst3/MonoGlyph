#pragma once

#include "screenBuffer.h"
#include "commonTypes.h"

#include <vector>

class Drawer
{
	using Line = Vector<float>;
	using Size = Point<int>;
	ScreenBuffer& sBuffer_;

public:
	Drawer(ScreenBuffer&) noexcept;

	void drawPixel(int, int, wchar_t) noexcept;
	void drawLine(int, int, int, int, wchar_t) noexcept;
	Size drawDynamicLetter(int, int, wchar_t) noexcept;
	void drawString(int, int, const wchar_t*) noexcept;
	void drawView(int, int, const std::vector<Line>&, wchar_t) noexcept;
	void drawBuffer(int, int, ScreenBuffer&) noexcept;

};
