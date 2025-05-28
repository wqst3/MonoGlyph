#include "stateInfinite.h"
#include "stateRestart.h"
#include "stateMenu.h"
#include "eventLoop.h"
#include "interfaceDrawer.h"

// === public methods ===
void StateInfinite::onEvent(MonoGlyph& MonoGlyph)
{
	EventLoop eventloop;

	int sigfd = MonoGlyph.signalFDHandler().fd().get();
	eventloop.addFd(sigfd, POLLIN, [&MonoGlyph, sigfd, this]() {
		signalfd_siginfo si;
		if (read(sigfd, &si, sizeof(si)) != sizeof(si)) {
			throw std::system_error(errno, std::generic_category(), "read sigfd failed");
		}
		MonoGlyph.onResize();
		draw(MonoGlyph);
	});

	int timerfd = MonoGlyph.timerFDHandler().fd().get();
	eventloop.addFd(timerfd, POLLIN, [&MonoGlyph, timerfd, this]() {
		uint64_t exp;
		if (read(timerfd, &exp, sizeof(exp)) != sizeof(exp)) {
			throw std::system_error(errno, std::generic_category(), "read timerfd failed");
		}
		draw(MonoGlyph);
	});

	eventloop.watchStdin([this, &MonoGlyph](char ch) {
		input(MonoGlyph, ch);
	});

	eventloop.run([&MonoGlyph]() {
		return MonoGlyph.currentState()->id() != StateID::Infinite;
	});
}

// === private methods ===
void StateInfinite::draw(MonoGlyph& MonoGlyph)
{
	InterfaceDrawer uiDrawer{MonoGlyph};

	uiDrawer.menu();

	MonoGlyph.screenBuffer().flush();
}

void StateInfinite::input(MonoGlyph& MonoGlyph, char ch)
{
	switch(ch)
	{
		case '\t':
			MonoGlyph.changeState(std::make_unique<StateRestart>());
			break;
		case 27: // esc	
			MonoGlyph.changeState(std::make_unique<StateMenu>());
			break;
		default:
			auto engFont = MonoGlyph.fonts().get("english");
			const std::string letters = engFont->getLetters();

			auto Glyph = engFont->getIfExists(ch);
			if (Glyph)
				MonoGlyph.newLetter(Glyph->segments == MonoGlyph.mainLetter().segments);

			break;
	}
}

