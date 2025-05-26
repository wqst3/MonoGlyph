#include "MonoGlyph.h"

#include <iostream>
#include <chrono>
#include <signal.h>
#include <sys/signalfd.h>
#include <sys/timerfd.h>
#include <poll.h>
#include <locale>

// === public methods ===
MonoGlyph::MonoGlyph()
  : currentState_(State::Loading)
  , terminal_()
  , sBuffer_(terminal_.size())
  , drawer_(sBuffer_)
  , fManager_()
  , signalFDHandler_(SIGWINCH)
  , timerFDHandler_(30)
{
	terminal_.altBuffer();
	terminal_.hideCur();
	terminal_.clear();
	terminal_.enableRawMode();

	std::setlocale(LC_ALL, "");
	std::wcout.imbue(std::locale(""));

	loadThread_ = std::thread([this]() {
		try {
			fManager_.load();
		} catch (const std::exception& e ) {
			std::cerr << "Font load error: " << e.what() << std::endl;
		}
		fontsLoaded_ = true;
	});
}

MonoGlyph::~MonoGlyph()
{
	if (loadThread_.joinable()) loadThread_.join();

	terminal_.disableRawMode();
	terminal_.restore();
}

int MonoGlyph::start()
{
	try {
		while (currentState_ != State::Exit)
		{
			switch (currentState_) 
			{
				case State::Loading:
					currentState_ = handleLoading();
					break;
				case State::Menu:
					currentState_ = handleMenu();
					break;
				case State::Infinite:
					currentState_ = handleTyping();
					break;
				default:
					currentState_ = State::Exit;
					break;
			}
		}

		return 0;
	}
	catch (const std::system_error& e) {
		std::cerr << "System error: " << e.what() << std::endl;
		return 1;
	}
	catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
		return 2;
	}
}


// === private methods ===
void MonoGlyph::onResize()
{
	Size size = terminal_.updateSize();
	sBuffer_.resize(size.x, size.y);
	terminal_.clear();
}

