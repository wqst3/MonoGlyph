#include "MonoGlyph.h"

MonoGlyph::State MonoGlyph::handleMenu()
{
	bool quit = true;

	eventLoop(
		[&](){ drawMenu(); },
		[&](){ onResize(); drawMenu(); },
		[&](char ch){ if (ch == 'q') quit = false; },
		[&](){ return quit; }
	);

	return State::Exit;
}

void MonoGlyph::drawMenu()
{
	Size size = terminal_.size();

	sBuffer_.syncBuffers();

	upMenuDraw(size);
	restartButton(size);

	mainLetter(size);
	leftLetter(size);
	rightLetter(size);

	sBuffer_.flush();
}

void MonoGlyph::upMenuDraw(Size size)
{
	// y: 0
	drawer_.drawString(1, 0, L"MonoGlyph");
	drawer_.drawLine(10, 0, size.x - 2, 0, L'─');
	drawer_.drawPixel(0, 0, L'╭');
	drawer_.drawPixel(size.x - 1, 0, L'╮');

	// y: 1
	drawer_.drawPixel(0, 1, L'│');
	drawer_.drawPixel(size.x - 1, 1, L'│');
	drawer_.drawString((size.x - 38) / 2, 1, L"english | timer letter | 15 30 60 120");

	// y: 2
	drawer_.drawPixel(0, 2, L'╰');
	drawer_.drawPixel(size.x - 1, 2, L'╯');
	drawer_.drawLine(1, 2, size.x - 2, 2, L'─');
}

void MonoGlyph::restartButton(Size size)
{
	drawer_.drawString((size.x - 5) / 2, size.y - 3, L"╭───╮");
	drawer_.drawString((size.x - 5) / 2, size.y - 2, L"│ ↻ │");
	drawer_.drawString((size.x - 5) / 2, size.y - 1, L"╰───╯");
}

void MonoGlyph::mainLetter(Size size)
{
	auto engFont = fManager_.get("english");

	ScreenBuffer mainLetter((size.x - 20) / 3, (size.y - 6) / 2);
	Drawer mainLetterDrawer(mainLetter);	

	auto mainGlyph = engFont->get('A');

	mainLetterDrawer.drawView(0, 0, mainGlyph.segments, L'╳');
	drawer_.drawBuffer((size.x - (size.x - 20) / 3) / 2, (size.y - (size.y - 6) / 2) / 2, mainLetter);
}

void MonoGlyph::leftLetter(Size size)
{
	auto engFont = fManager_.get("english");

	ScreenBuffer letter((size.x - 20) / 4.5f, (size.y - 6) / 3);
	Drawer letterDrawer(letter);

	auto leftGlyph = engFont->get('B');

	letterDrawer.drawView(0, 0, leftGlyph.segments, L'╳');
	drawer_.drawBuffer((size.x - (size.x - 20) / 4.5f) / 6, (size.y - (size.y - 6) / 3) / 2, letter);
}

void MonoGlyph::rightLetter(Size size)
{
	auto engFont = fManager_.get("english");
	
	ScreenBuffer letter((size.x - 20) / 4.5f, (size.y - 6) / 3);
	Drawer letterDrawer(letter);

	auto rightGlyph = engFont->get('C');

	letterDrawer.drawView(0, 0, rightGlyph.segments, L'╳');
	drawer_.drawBuffer(size.x - (size.x - 20) / 4.5f - (size.x - (size.x - 20) / 4.5f) / 6, (size.y - (size.y - 6) / 3) / 2, letter);
}

void MonoGlyph::onInput(char ch)
{}

bool MonoGlyph::menuDone()
{
	using namespace std::chrono;
	static auto startTime = steady_clock::now();

	return (steady_clock::now() - startTime) > seconds(5);
}

