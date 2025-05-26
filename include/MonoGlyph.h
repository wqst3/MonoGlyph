#pragma once 

#include "commonTypes.h"
#include "terminal.h"
#include "screenBuffer.h"
#include "drawer.h"
#include "fontManager.h"
#include "signalFD.hpp"
#include "timerFD.hpp"

#include <functional>
#include <thread>

class MonoGlyph
{
	using Line = Vector<float>;
	using Size = Point<int>;

	enum class State { Loading, Menu, Infinite, Restart, Exit };
	State currentState_;

	Terminal terminal_;
	ScreenBuffer sBuffer_;
	Drawer drawer_;
	FontManager fManager_;

	SignalFDHandler signalFDHandler_;
	TimerFDHandler timerFDHandler_;

	std::thread loadThread_;
	std::atomic<bool> fontsLoaded_{false};

	char mainLetter_ = ' ', leftLetter_ = ' ', rightLetter_ = ' ';
	
	// loading
	State handleLoading();
	void drawLoadingFrame(bool);
	bool loadingDone();
	
	// menu
	State handleMenu();
	void updateLetters();
	void newLetter();
	void drawMenu();
	void upMenuDraw(Size);
	void restartButton(Size);
	void mainLetter(Size);
	void leftLetter(Size);
	void rightLetter(Size);
	void menuInput(char);
	bool menuDone();

	// typing
	State handleTyping();
	
	void onResize();

public:
	MonoGlyph();

	explicit MonoGlyph(const MonoGlyph&) = delete;
	MonoGlyph& operator=(const MonoGlyph&) = delete;

	~MonoGlyph() noexcept;

	int start();

};

