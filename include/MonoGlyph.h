#pragma once 

#include "commonTypes.h"
#include "terminal.h"
#include "screenBuffer.h"
#include "drawer.h"

#include <atomic>

class MonoGlyph
{
	using Line = Vector<float>;
	using Size = Point<int>;

	Terminal terminal_;
	ScreenBuffer sBuffer_;
	Drawer drawer_;

	void timer(int) const noexcept;

public:
	MonoGlyph();

	MonoGlyph(const MonoGlyph&) = delete;
	MonoGlyph& operator=(const MonoGlyph&) = delete;

	~MonoGlyph();

	int start();

};

