#include "MonoGlyph.h"

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
  , drawer_(sBuffer_)
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

		ScreenBuffer rhombus(10, 10);
		Drawer rhombusDrawer(rhombus);

		rhombusDrawer.drawLine(0, 4, 4, 0, '*');
		rhombusDrawer.drawLine(5, 0, 9, 4, '*');
		rhombusDrawer.drawLine(9, 5, 5, 9, '*');
		rhombusDrawer.drawLine(4, 9, 0, 5, '*');

		drawer_.drawBuffer((tSize.x - 10) / 2, (tSize.y - 10) / 2, rhombus);
		sBuffer_.flush();

		using namespace std::chrono;
		auto end = high_resolution_clock::now() + seconds(15);
		while (high_resolution_clock::now() < end)
		{
			if (resized.exchange(false)) {
				Size tSize = terminal_.updateSize();
				sBuffer_.resize(tSize.x, tSize.y);

				sBuffer_.clear();
				drawer_.drawBuffer((tSize.x - 10) / 2, (tSize.y - 10) / 2, rhombus);
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

