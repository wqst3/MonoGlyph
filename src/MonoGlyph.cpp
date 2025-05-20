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
	initTimerFD(5);
}

MonoGlyph::~MonoGlyph()
{
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
	if (sigprocmask(SIG_BLOCK, &mask, nullptr) == -1) {
		throw std::system_error(errno, std::generic_category(), "sigprocmask failed");
	}

	int fd = signalfd(-1, &mask, SFD_CLOEXEC);
	if (fd == -1) {
		throw std::system_error(errno, std::generic_category(), "signalfd failed");
	}

	sigfd_ = FileDescriptor(fd);
}

void MonoGlyph::initTimerFD(unsigned int fps)
{
	int fd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC);
	if (fd == -1) {
		throw std::system_error(errno, std::generic_category(), "timerfd_create failed");
	}

	itimerspec its{};
	its.it_value.tv_nsec = 1'000'000'000 / fps;
	its.it_interval = its.it_value;
	if (timerfd_settime(fd, 0, &its, nullptr) == -1) {
		throw std::system_error(errno, std::generic_category(), "timerfd_settime failed");
	}

	timerfd_ = FileDescriptor(fd);
}


void MonoGlyph::eventLoop(std::function<void()> onTimeout,
			  std::function<void()> onResize,
			  std::function<bool()> shouldQuit)
{
	struct pollfd fds[2];
	fds[0].fd = sigfd_.get(); fds[0].events = POLLIN;
	fds[1].fd = timerfd_.get(); fds[1].events = POLLIN;

	while (!shouldQuit()) {
		if (poll(fds, 2, -1) == -1) {
			if (errno == EINTR) continue;
			throw std::system_error(errno, std::generic_category(), "poll failed");
		}
		if (fds[0].revents & POLLIN) {
			signalfd_siginfo si;
			if (read(sigfd_.get(), &si, sizeof(si)) != sizeof(si)) {
				throw std::system_error(errno, std::generic_category(), "read sigfd failed");
			}
			onResize();
		}
		if (fds[1].revents & POLLIN) {
			uint64_t exp;
			if (read(timerfd_.get(), &exp, sizeof(exp)) != sizeof(exp)) {
				throw std::system_error(errno, std::generic_category(), "read timerfd failed");
			}
			onTimeout();
		}
	}
}

