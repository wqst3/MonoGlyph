#pragma once 

#include "commonTypes.h"
#include "terminal.h"
#include "screenBuffer.h"

#include <atomic>

class MonoGlyph
{
	using Size = Point;

	Terminal terminal_;
	ScreenBuffer sBuffer_;

	void timer(int) const noexcept;

public:
	MonoGlyph();

	MonoGlyph(const MonoGlyph&) = delete;
	MonoGlyph& operator=(const MonoGlyph&) = delete;

	~MonoGlyph();

	int start();

};

