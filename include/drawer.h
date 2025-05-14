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

	void drawPixel(int, int, char) noexcept;
	void drawLine(int, int, int, int, char) noexcept;
	void drawLetter(int, int, char) noexcept;
	void drawString(int, int, const char*) noexcept;
	void drawView(int, int, std::vector<Line>&, char) noexcept;
	void drawBuffer(int, int, ScreenBuffer&) noexcept;

};
