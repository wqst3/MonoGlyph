#pragma once

#include "commonTypes.h"

#include <termios.h>

class Terminal
{
	using Size = Point;

	termios orig_termios_;
	termios raw_;

	Size size_;

public:
	Terminal();

	void updateSize();

	void enableRawMode();
	void disableRawMode();

	void altBuffer();
	void hideCur();
	void clear();
	void restore();

	Size size() const noexcept { return size_; }

};

