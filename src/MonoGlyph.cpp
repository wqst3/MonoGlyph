#include "MonoGlyph.h"
#include "terminal.h"

#include <iostream>
#include <chrono>
#include <signal.h>

namespace {
	std::atomic<bool> resized(false);

	void handle_winch(int /*signum*/) {
    	resized.store(true);
	}
}

// === public methods ===
MonoGlyph::MonoGlyph()
{
	terminal_.clear();
	terminal_.enableRawMode();

	struct sigaction sa{};
	sa.sa_handler = handle_winch;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGWINCH, &sa, nullptr);
}

MonoGlyph::~MonoGlyph()
{
	terminal_.disableRawMode();
	terminal_.restore();
}

int MonoGlyph::start()
{
	try {

		Size size = terminal_.size();	
		std::cout << "x: " << size.x << ", y: " << size.y << std::endl;

		using namespace std::chrono;

		auto end = high_resolution_clock::now() + seconds(60);
		while (high_resolution_clock::now() < end)
		{
			if (resized.exchange(false)) {
				terminal_.updateSize();
				terminal_.clear();

				size = terminal_.size();
				std::cout << "x: " << size.x << ", y: " << size.y << std::endl;
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

