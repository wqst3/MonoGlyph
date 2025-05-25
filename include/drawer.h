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

	void drawPixel(int, int, const Pixel&) noexcept;
	void drawLine(int, int, int, int, const Pixel&) noexcept;
	Size drawDynamicLetter(int, int, const Pixel&) noexcept;
	void drawString(int, int, const Pixel*) noexcept;
	void drawString(int, int, const wchar_t*, Color = Color::Default, BgColor = BgColor::Default) noexcept;
	void drawView(int, int, const std::vector<Line>&, const Pixel&) noexcept;
	void drawBuffer(int, int, ScreenBuffer&) noexcept;

};
