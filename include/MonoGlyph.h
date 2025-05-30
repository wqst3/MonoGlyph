#pragma once 

#include "commonTypes.h"
#include "terminal.h"
#include "screenBuffer.h"
#include "drawer.h"
#include "fontManager.h"
#include "signalFD.hpp"
#include "timerFD.hpp"
#include "state.h"

#include <random>

class MonoGlyph
{
	using Line = Vector<float>;
	using Size = Point<int>;

	bool correctLetterInput_;

	std::unique_ptr<State> currentState_;

	Terminal terminal_;
	ScreenBuffer sBuffer_;
	Drawer drawer_;
	FontManager fManager_;

	std::random_device rd_;
	std::mt19937 gen_;

	SignalFDHandler signalFDHandler_;
	TimerFDHandler timerFDHandler_;

	Glyph leftLetter_;
	Glyph mainLetter_;
	Glyph rightLetter_;

public:
	MonoGlyph();

	explicit MonoGlyph(const MonoGlyph&) = delete;
	MonoGlyph& operator=(const MonoGlyph&) = delete;

	~MonoGlyph() noexcept;

	bool correctLetterInput() const noexcept;

	int start();
	void onResize();
	void updateLetters();
	void newLetter(bool);

	void changeState(std::unique_ptr<State>);

	State* currentState() noexcept;

	Terminal& terminal() noexcept;
	ScreenBuffer& screenBuffer() noexcept;
	Drawer& drawer() noexcept;
	FontManager& fonts() noexcept;

	SignalFDHandler& signalFDHandler() noexcept;
	TimerFDHandler& timerFDHandler() noexcept;

	Glyph& leftLetter() noexcept;
	Glyph& mainLetter() noexcept;
	Glyph& rightLetter() noexcept;

};

