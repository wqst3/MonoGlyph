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
  : terminal_()
  , sBuffer_(terminal_.size())
{
	terminal_.altBuffer();
	terminal_.hideCur();
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
		Size tSize = terminal_.size();
		sBuffer_.at(tSize.x / 2, tSize.y / 2) = '#';
		sBuffer_.flush();
		
		using namespace std::chrono;
		auto end = high_resolution_clock::now() + seconds(15);
		while (high_resolution_clock::now() < end)
		{
			if (resized.exchange(false)) {
				terminal_.updateSize();

				tSize = terminal_.size();
				sBuffer_.resize(tSize.x, tSize.y);

				sBuffer_.at(tSize.x / 2, tSize.y / 2) = '#';

				terminal_.clear();
				sBuffer_.flush();
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

