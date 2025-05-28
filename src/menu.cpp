#include "MonoGlyph.h"
#include "eventLoop.h"


#include <cstring>

void MonoGlyph::updateLetters()
{
	auto engFont = fManager_.get("english");
	const std::string letters = engFont->getLetters();

	size_t len = letters.size();
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, len - 1);

	leftLetter_ = ' ';
	mainLetter_ = letters[dis(gen)];
	rightLetter_ = letters[dis(gen)];
}

void MonoGlyph::newLetter()
{
	auto engFont = fManager_.get("english");
	const std::string letters = engFont->getLetters();

	size_t len = letters.size();
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, len - 1);
	
	leftLetter_ = mainLetter_;
	mainLetter_ = rightLetter_;
	rightLetter_ = letters[dis(gen)];
}

MonoGlyph::State MonoGlyph::handleMenu()
{
	updateLetters();

	EventLoop eventloop;

	int sigfd = signalFDHandler_.fd().get();
	eventloop.addFd(sigfd, POLLIN, [this, sigfd]() {
		signalfd_siginfo si;
		if (read(sigfd, &si, sizeof(si)) != sizeof(si)) {
			throw std::system_error(errno, std::generic_category(), "read sigfd failed");
		}
		onResize();
		drawMenu();
	});

	int timerfd = timerFDHandler_.fd().get();
	eventloop.addFd(timerfd, POLLIN, [this, timerfd]() {
		uint64_t exp;
		if (read(timerfd, &exp, sizeof(exp)) != sizeof(exp)) {
			throw std::system_error(errno, std::generic_category(), "read timerfd failed");
		}
		drawMenu();
	});

	eventloop.watchStdin([this](char ch) {
		menuInput(ch);
	});

	eventloop.run([this]() {
		return currentState_ == State::Exit;
	});

	return State::Exit;
}

void MonoGlyph::drawMenu()
{
	Size size = terminal_.size();
	
	upMenuDraw(size);
	restartButton(size);

	mainLetter(size);
	rightLetter(size);
	leftLetter(size);

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
	drawer_.drawString((size.x - 46) / 2, 1, L"english | timer letter infinite | 15 30 60 120");

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

	if (currentState_ == State::Restart)
	{
		drawer_.drawPixel((size.x - 1) / 2, size.y - 2, Pixel{L'↻', Color::Black, BgColor::White});
	}
}

void MonoGlyph::mainLetter(Size size)
{
	auto engFont = fManager_.get("english");

	ScreenBuffer mainLetter((size.x - 20) / 3, (size.y - 6) / 2);
	Drawer mainLetterDrawer(mainLetter);	

	if (mainLetter_ != ' ') {
		auto mainGlyph = engFont->get(mainLetter_);
		mainLetterDrawer.drawView(0, 0, mainGlyph.segments, L'-');
	}

	drawer_.drawBuffer((size.x - (size.x - 20) / 3) / 2, (size.y - (size.y - 6) / 2) / 2, mainLetter);
}

void MonoGlyph::leftLetter(Size size)
{
	auto engFont = fManager_.get("english");

	ScreenBuffer letter((size.x - 20) / 4.5f, (size.y - 6) / 3);
	Drawer letterDrawer(letter);

	if (leftLetter_ != ' ') {
		auto leftGlyph = engFont->get(leftLetter_);
		letterDrawer.drawView(0, 0, leftGlyph.segments, L'-');
	}

	drawer_.drawBuffer((size.x - (size.x - 20) / 4.5f) / 6, (size.y - (size.y - 6) / 3) / 2, letter);
}

void MonoGlyph::rightLetter(Size size)
{
	auto engFont = fManager_.get("english");

	ScreenBuffer letter((size.x - 20) / 4.5f, (size.y - 6) / 3);
	Drawer letterDrawer(letter);

	if (rightLetter_ != ' ') {
		auto rightGlyph = engFont->get(rightLetter_);
		letterDrawer.drawView(0, 0, rightGlyph.segments, L'-');
	}

	drawer_.drawBuffer(size.x - (size.x - 20) / 4.5f - (size.x - (size.x - 20) / 4.5f) / 6, (size.y - (size.y - 6) / 3) / 2, letter);
}

void MonoGlyph::menuInput(char ch)
{
	switch (currentState_)
	{
		case State::Restart:
			switch(ch)
			{
				case '\r':
				case '\n':
					updateLetters();
					currentState_ = State::Menu;
					break;

				case 27: // esc
					currentState_ = State::Menu;
					break;
				default:
					break;
			}
			break;

		case State::Menu:
			switch(ch)
			{
				case '\t':
					currentState_ = State::Restart;
					break;
				case 27: // esc
					currentState_ = State::Exit;
					break;
				case 'i':
					currentState_ = State::Infinite;
				default:
					break;
			}
			break;

		case State::Infinite:
			switch(ch)
			{
				case '\t':
					currentState_ = State::Restart;
					break;
				case 27: // esc
					updateLetters();
					currentState_ = State::Menu;
					break;
				default:
					if (ch == mainLetter_)
						newLetter();
					break;
			}
			break;
		default:
			break;
	}
}

bool MonoGlyph::menuDone()
{
	using namespace std::chrono;
	static auto startTime = steady_clock::now();

	return (steady_clock::now() - startTime) > seconds(5);
}

