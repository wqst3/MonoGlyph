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
  , fManager_()
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
		auto font = fManager_.get("english");
		if (!font) {
			std::cerr << "Шрифт 'english' не найден\n";
			return 0;
		}

		const Glyph* g = font->getIfExists('A');
		if (g)
			drawer_.drawView(0, 0, g->segments, '*');
		sBuffer_.flush();

		using namespace std::chrono;
		auto end = high_resolution_clock::now() + seconds(15);
		while (high_resolution_clock::now() < end)
		{
			if (resized.exchange(false)) {
				Size tSize = terminal_.updateSize();
				sBuffer_.resize(tSize.x, tSize.y);

				drawer_.drawView(0, 0, g->segments, '*');
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

