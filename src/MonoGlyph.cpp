#include "MonoGlyph.h"
#include "terminal.h"

#include <iostream>
#include <chrono>

// === public methods ===
MonoGlyph::MonoGlyph()
{
	terminal_.clear();
	terminal_.enableRawMode();
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

		auto end = high_resolution_clock::now() + seconds(3);
		while (high_resolution_clock::now() < end)
		{

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

