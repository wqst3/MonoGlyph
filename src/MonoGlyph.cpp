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

void MonoGlyph::eventLoop(std::function<void()> onTimeout,
			  std::function<void()> onResize,
			  std::function<void(char)> onInput,
			  std::function<bool()> shouldQuit)
{
	struct pollfd fds[3];
	fds[0].fd = signalFDHandler_.fd().get(); fds[0].events = POLLIN;
	fds[1].fd = timerFDHandler_.fd().get(); fds[1].events = POLLIN;
	fds[2].fd = STDIN_FILENO; fds[2].events = POLLIN;

	while (shouldQuit()) {
		if (poll(fds, 3, -1) == -1) {
			if (errno == EINTR) continue;
			throw std::system_error(errno, std::generic_category(), "poll failed");
		}
		if (fds[0].revents & POLLIN) {
			signalfd_siginfo si;
			if (read(signalFDHandler_.fd().get(), &si, sizeof(si)) != sizeof(si)) {
				throw std::system_error(errno, std::generic_category(), "read sigfd failed");
			}
			onResize();
		}
		if (fds[1].revents & POLLIN) {
			uint64_t exp;
			if (read(timerFDHandler_.fd().get(), &exp, sizeof(exp)) != sizeof(exp)) {
				throw std::system_error(errno, std::generic_category(), "read timerfd failed");
			}
			onTimeout();
		}
		if (fds[2].revents & POLLIN) {
            		char ch;
            		ssize_t len = read(STDIN_FILENO, &ch, 1);
            		if (len == 1) {
                		onInput(ch);
            		} else if (len == -1) {
                		throw std::system_error(errno, std::generic_category(), "read stdin failed");
            		}
        	}
	}
}

