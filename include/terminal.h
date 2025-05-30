#pragma once

#include "commonTypes.h"

#include <termios.h>
#include <cstddef>

class Terminal
{
	using Size = Point<int>;

	termios orig_termios_;
	termios raw_;

	Size size_;

	void safeWrite(int, const char*, size_t, const char*);

public:
	Terminal();

	Size updateSize();

	void enableRawMode();
	void disableRawMode();

	void altBuffer();
	void hideCur();
	void clear();
	void restore();

	Size size() const noexcept { return size_; }

};

