#include "stateRestart.h"

#include "eventLoop.h"
#include "interfaceDrawer.h"
#include "stateMenu.h"

// === public methods ===
void StateRestart::onEvent(MonoGlyph &MonoGlyph) {
  EventLoop eventloop;

  int sigfd = MonoGlyph.signalFDHandler().fd().get();
  eventloop.addFd(sigfd, POLLIN, [&MonoGlyph, sigfd, this]() {
    signalfd_siginfo si;
    if (read(sigfd, &si, sizeof(si)) != sizeof(si)) {
      throw std::system_error(errno, std::generic_category(),
                              "read sigfd failed");
    }
    MonoGlyph.onResize();
    draw(MonoGlyph);
  });

  int timerfd = MonoGlyph.timerFDHandler().fd().get();
  eventloop.addFd(timerfd, POLLIN, [&MonoGlyph, timerfd, this]() {
    uint64_t exp;
    if (read(timerfd, &exp, sizeof(exp)) != sizeof(exp)) {
      throw std::system_error(errno, std::generic_category(),
                              "read timerfd failed");
    }
    draw(MonoGlyph);
  });

  eventloop.watchStdin([this, &MonoGlyph](char ch) { input(MonoGlyph, ch); });

  eventloop.run([&MonoGlyph]() {
    return MonoGlyph.currentState()->id() != StateID::Restart;
  });
}

// === private methods ===
void StateRestart::draw(MonoGlyph &MonoGlyph) {
  InterfaceDrawer uiDrawer{MonoGlyph};

  uiDrawer.menu();

  MonoGlyph.screenBuffer().flush();
}

void StateRestart::input(MonoGlyph &MonoGlyph, char ch) {
  switch (ch) {
    case '\r':
    case '\n':
      MonoGlyph.updateLetters();
      MonoGlyph.changeState(std::make_unique<StateMenu>());
      break;
    case 27:  // esc
      MonoGlyph.changeState(std::make_unique<StateMenu>());
      break;
    default:
      break;
  }
}
