#pragma once

#include "MonoGlyph.h"

class InterfaceDrawer
{
	using Line = Vector<float>;
	using Size = Point<int>;

	MonoGlyph& MonoGlyph_;

public:
	InterfaceDrawer(MonoGlyph&);

	void menu();

	void upMenu();
	void restartButton();
	void mainLetter();
	void leftLetter();
	void rightLetter();
	void timer(int);

};

