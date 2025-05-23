#pragma once 

#include "commonTypes.h"
#include "terminal.h"
#include "screenBuffer.h"
#include "drawer.h"
#include "fontManager.h"
#include "fileDescriptor.h"

#include <functional>
#include <thread>

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

	FileDescriptor sigfd_;
	FileDescriptor timerfd_;

    	std::thread loadThread_;
    	std::atomic<bool> fontsLoaded_{false};
	
	// loading
    	State handleLoading();
	void drawLoadingFrame(bool);
	bool loadingDone();
	
	// menu
	State handleMenu();
	void drawMenu();
	void upMenuDraw(Size);
	void restartButton(Size);
	void mainLetter(Size);
	void leftLetter(Size);
	void rightLetter(Size);
	void onInput(char);
	bool menuDone();

	// typing
    	State handleTyping();
	
	void onResize();

	void initSignalFD();
	void initTimerFD(unsigned int);

	void eventLoop(std::function<void()>,
		       std::function<void()>,
		       std::function<void(char)>,
		       std::function<bool()>);

public:
	MonoGlyph();

	explicit MonoGlyph(const MonoGlyph&) = delete;
	MonoGlyph& operator=(const MonoGlyph&) = delete;

	~MonoGlyph() noexcept;

	int start();

};

