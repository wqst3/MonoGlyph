#include "drawer.h"

#include <cmath>

// === public methods ===
Drawer::Drawer(ScreenBuffer& sb) noexcept : sBuffer_(sb) {}


void Drawer::drawPixel(int x, int y, char c) noexcept
{
	Size size = sBuffer_.size();

	if (x < size.x && x >= 0 && y < size.y && y >= 0)
		sBuffer_.at(x, y) = c;
}

void Drawer::drawLine(int x1, int y1, int x2, int y2, char c) noexcept
{
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


void Drawer::drawLetter(int x, int y, char letter) noexcept
{
	drawPixel(x, y, letter);
}

void Drawer::drawString(int x, int y, const char* s) noexcept
{
	while (*s)
	{
		drawPixel(x, y, *s);
		++s;
		++x;
	}
}

//void Drawer::drawView(int x, int y, std::vector<Line>& lines, char c)
//{
//	for (const auto& line : lines)
//		drawLine(line.x1, line.y1, line.x2, line.y2, c)
//}

void Drawer::drawBuffer(int x, int y, ScreenBuffer& sb) noexcept
{
	Size sbSize = sb.size();

	for (int sbY = 0; sbY <  sbSize.y; ++sbY)
		for (int sbX = 0; sbX < sbSize.x; ++sbX)
			drawPixel(x + sbX, y + sbY, sb.at(sbX, sbY));
}

