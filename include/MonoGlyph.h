#pragma once 

#include "commonTypes.h"
#include "terminal.h"

class MonoGlyph
{
	using Size = Point;

	Terminal terminal_;

	void timer(int) const noexcept;

public:
	MonoGlyph();
	~MonoGlyph();

	int start();

};

