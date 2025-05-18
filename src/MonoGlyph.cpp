#include "MonoGlyph.h"

#include <iostream>
#include <chrono>
#include <signal.h>
#include <sys/signalfd.h>
#include <sys/timerfd.h>
#include <poll.h>

// === public methods ===
MonoGlyph::MonoGlyph()
  : currentState_(State::Loading)
  , terminal_()
  , sBuffer_(terminal_.size())
  , drawer_(sBuffer_)
  , fManager_()
{
	terminal_.altBuffer();
	terminal_.hideCur();
	terminal_.clear();
	terminal_.enableRawMode();

	initSignalFD();
	initTimerFD(30);
}

MonoGlyph::~MonoGlyph()
{
	if (sigfd_ != -1) close(sigfd_);
	if (timerfd_ != -1) close(timerfd_);

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
				case State::Typing:
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
void MonoGlyph::timer(int sec) const noexcept
{
	using namespace std::chrono;

	auto end = high_resolution_clock::now() + seconds(sec);
	while (high_resolution_clock::now() < end) {}
}


MonoGlyph::State MonoGlyph::handleMenu()
{
	return State::Exit;
}

MonoGlyph::State MonoGlyph::handleLoading()
{
	eventLoop(
		[&](){ drawLoadingFrame(); },
		[&](){ onResize(); drawLoadingFrame(); },
		[&](){ return loadingDone(); }
	);
	return State::Menu;
}

MonoGlyph::State MonoGlyph::handleTyping()
{
	return State::Exit;
}


void MonoGlyph::drawLoadingFrame()
{
	static int frame = 0;
	const char spinner[] = {'|', '/', '-', '\\'};

	drawer_.drawPixel(0, 0, spinner[frame++ % 4]);

	terminal_.clear();
	sBuffer_.flush();
}

bool MonoGlyph::loadingDone()
{
	using namespace std::chrono;
	static auto startTime = steady_clock::now();
	return (steady_clock::now() - startTime) > seconds(5);
}

void MonoGlyph::onResize()
{
	Size size = terminal_.updateSize();
	sBuffer_.resize(size.x, size.y);
}


void MonoGlyph::initSignalFD()
{
	sigset_t mask{};
	sigemptyset(&mask);
	sigaddset(&mask, SIGWINCH);
	sigprocmask(SIG_BLOCK, &mask, nullptr);
	sigfd_ = signalfd(-1, &mask, SFD_CLOEXEC);
}

void MonoGlyph::initTimerFD(unsigned int fps)
{
	timerfd_ = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC);
	itimerspec its{};
	its.it_value.tv_nsec = 1'000'000'000 / fps;
	its.it_interval = its.it_value;
	timerfd_settime(timerfd_, 0, &its, nullptr);
}

void MonoGlyph::eventLoop(std::function<void()> onTimeout,
			  std::function<void()> onResize,
			  std::function<bool()> shouldQuit)
{
	struct pollfd fds[2];
	fds[0].fd = sigfd_; fds[0].events = POLLIN;
	fds[1].fd = timerfd_; fds[1].events = POLLIN;

	while (!shouldQuit()) {
		poll(fds, 2, -1);
		if (fds[0].revents & POLLIN) {
			signalfd_siginfo si; read(sigfd_, &si, sizeof(si));
			onResize();
		}
		if (fds[1].revents & POLLIN) {
			uint64_t exp; read(timerfd_, &exp, sizeof(exp));
			onTimeout();
		}
	}
}

