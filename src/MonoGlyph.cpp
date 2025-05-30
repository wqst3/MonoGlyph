#include "MonoGlyph.h"

#include <poll.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <sys/timerfd.h>

#include <iostream>
#include <locale>

#include "stateMenu.h"

// === public methods ===
MonoGlyph::MonoGlyph()
    : terminal_(),
      sBuffer_(terminal_.size()),
      drawer_(sBuffer_),
      fManager_(),
      gen_(rd_()),
      signalFDHandler_(SIGWINCH),
      timerFDHandler_(30) {
  terminal_.altBuffer();
  terminal_.hideCur();
  terminal_.clear();
  terminal_.enableRawMode();

  std::setlocale(LC_ALL, "");
  std::wcout.imbue(std::locale(""));

  try {
    fManager_.load();
  } catch (const std::exception &e) {
    std::cerr << "Font load error: " << e.what() << std::endl;
  }

  changeState(std::make_unique<StateMenu>());
  updateLetters();
}

MonoGlyph::~MonoGlyph() {
  terminal_.disableRawMode();
  terminal_.restore();
}

bool MonoGlyph::correctLetterInput() const noexcept {
  return correctLetterInput_;
}

int MonoGlyph::start() {
  try {
    while (currentState_) {
      currentState_->onEvent(*this);
    }
    return 0;
  } catch (const std::system_error &e) {
    std::cerr << "System error: " << e.what() << std::endl;
    return 1;
  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 2;
  }
}

void MonoGlyph::onResize() {
  Size size = terminal_.updateSize();
  sBuffer_.resize(size.x, size.y);
  terminal_.clear();
}

void MonoGlyph::updateLetters() {
  auto engFont = fManager_.get("english");
  const std::string letters = engFont->getLetters();

  size_t len = letters.size();
  std::uniform_int_distribution<> dis(0, len - 1);

  mainLetter_ = engFont->get(letters[dis(gen_)]);
  rightLetter_ = engFont->get(letters[dis(gen_)]);
  leftLetter_.segments.clear();
}

void MonoGlyph::newLetter(bool correct) {
  correctLetterInput_ = correct;

  auto engFont = fManager_.get("english");
  const std::string letters = engFont->getLetters();

  size_t len = letters.size();
  std::uniform_int_distribution<> dis(0, len - 1);

  leftLetter_ = mainLetter_;
  mainLetter_ = rightLetter_;
  rightLetter_ = engFont->get(letters[dis(gen_)]);
}

void MonoGlyph::changeState(std::unique_ptr<State> newState) {
  currentState_ = std::move(newState);
  if (currentState_) currentState_->onEnter(*this);
}

State *MonoGlyph::currentState() noexcept { return currentState_.get(); }

Terminal &MonoGlyph::terminal() noexcept { return terminal_; }
ScreenBuffer &MonoGlyph::screenBuffer() noexcept { return sBuffer_; }
Drawer &MonoGlyph::drawer() noexcept { return drawer_; }
FontManager &MonoGlyph::fonts() noexcept { return fManager_; }

SignalFDHandler &MonoGlyph::signalFDHandler() noexcept {
  return signalFDHandler_;
}
TimerFDHandler &MonoGlyph::timerFDHandler() noexcept { return timerFDHandler_; }

Glyph &MonoGlyph::leftLetter() noexcept { return leftLetter_; }
Glyph &MonoGlyph::mainLetter() noexcept { return mainLetter_; }
Glyph &MonoGlyph::rightLetter() noexcept { return rightLetter_; }
