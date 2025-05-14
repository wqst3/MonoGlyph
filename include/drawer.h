#pragma once

#include "screenBuffer.h"
#include "commonTypes.h"

class Drawer
{
	using Size = Point;
	ScreenBuffer& sBuffer_;

public:
	Drawer(ScreenBuffer&) noexcept;

	void drawPixel(int, int, char) noexcept;
	void drawLine(int, int, int, int, char) noexcept;
	void drawLetter(int, int, char) noexcept;
	void drawString(int, int, const char*) noexcept;

};
