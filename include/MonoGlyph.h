#pragma once 

#include "commonTypes.h"
#include "terminal.h"
#include "screenBuffer.h"
#include "drawer.h"
#include "fontManager.h"

#include <functional>

class MonoGlyph
{
	using Line = Vector<float>;
	using Size = Point<int>;

	enum class State { Loading, Menu, Typing, Exit };
	State currentState_;

	Terminal terminal_;
	ScreenBuffer sBuffer_;
	Drawer drawer_;
	FontManager fManager_;

	int sigfd_;
	int timerfd_;

	void timer(int) const noexcept;

	State handleMenu();
    	State handleLoading();
    	State handleTyping();

	void drawLoadingFrame();
	bool loadingDone();
	void onResize();

	void initSignalFD();
	void initTimerFD(unsigned int);
	void eventLoop(std::function<void()>,
		       std::function<void()>,
		       std::function<bool()>);

public:
	MonoGlyph();

	MonoGlyph(const MonoGlyph&) = delete;
	MonoGlyph& operator=(const MonoGlyph&) = delete;

	~MonoGlyph();

	int start();

};

