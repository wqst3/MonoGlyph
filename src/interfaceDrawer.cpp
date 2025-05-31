#include "interfaceDrawer.h"

#include "color.h"
#include "pixel.h"
#include "stateID.h"

// === public methods ===
InterfaceDrawer::InterfaceDrawer(MonoGlyph &MonoGlyph)
    : MonoGlyph_(MonoGlyph) {}

void InterfaceDrawer::menu() {
  upMenu();
  restartButton();
  leftLetter();
  mainLetter();
  rightLetter();
}

void InterfaceDrawer::upMenu() {
  Drawer &drawer = MonoGlyph_.drawer();
  Size size = MonoGlyph_.terminal().size();
  // y: 0
  drawer.drawString(1, 0, L"MonoGlyph");
  drawer.drawLine(10, 0, size.x - 2, 0, L'─');
  drawer.drawPixel(0, 0, L'╭');
  drawer.drawPixel(size.x - 1, 0, L'╮');

  // y: 1
  drawer.drawPixel(0, 1, L'│');
  drawer.drawPixel(size.x - 1, 1, L'│');

  int x = (size.x - 46) / 2;
  int y = 1;

  drawer.drawString(x, y, L"english | timer letter infinite | 15 30 60 120");

  switch (MonoGlyph_.currentState()->id()) {
  case StateID::Infinite:
    drawer.drawString(x + 23, y, L"infinite", Color::Black, BgColor::White);
    break;
  default:
    break;
  }

  // y: 2
  drawer.drawPixel(0, 2, L'╰');
  drawer.drawPixel(size.x - 1, 2, L'╯');
  drawer.drawLine(1, 2, size.x - 2, 2, L'─');
}

void InterfaceDrawer::restartButton() {
  Drawer &drawer = MonoGlyph_.drawer();
  Size size = MonoGlyph_.terminal().size();

  drawer.drawString((size.x - 5) / 2, size.y - 3, L"╭───╮");
  drawer.drawString((size.x - 5) / 2, size.y - 2, L"│ ↻ │");
  drawer.drawString((size.x - 5) / 2, size.y - 1, L"╰───╯");

  if (MonoGlyph_.currentState()->id() == StateID::Restart) {
    drawer.drawPixel((size.x - 1) / 2, size.y - 2,
                     Pixel{L'↻', Color::Black, BgColor::White});
  }
}

void InterfaceDrawer::mainLetter() {
  Drawer &drawer = MonoGlyph_.drawer();
  Size size = MonoGlyph_.terminal().size();
  Glyph &mainGlyph = MonoGlyph_.mainLetter();

  ScreenBuffer mainLetter((size.x - 20) / 3, (size.y - 6) / 2);
  Drawer mainLetterDrawer(mainLetter);

  mainLetterDrawer.drawView(0, 0, mainGlyph.segments, L'-');

  drawer.drawBuffer((size.x - (size.x - 20) / 3) / 2,
                    (size.y - (size.y - 6) / 2) / 2, mainLetter);
}

void InterfaceDrawer::leftLetter() {
  Drawer &drawer = MonoGlyph_.drawer();
  Size size = MonoGlyph_.terminal().size();
  Glyph &leftGlyph = MonoGlyph_.leftLetter();

  ScreenBuffer letter((size.x - 20) / 4.5f, (size.y - 6) / 3);
  Drawer letterDrawer(letter);

  if (MonoGlyph_.correctLetterInput())
    letterDrawer.drawView(0, 0, leftGlyph.segments,
                          Pixel{L'-', Color::Green, BgColor::Default});
  else
    letterDrawer.drawView(0, 0, leftGlyph.segments,
                          Pixel{L'-', Color::Red, BgColor::Default});

  drawer.drawBuffer((size.x - (size.x - 20) / 4.5f) / 6,
                    (size.y - (size.y - 6) / 3) / 2, letter);
}

void InterfaceDrawer::rightLetter() {
  Drawer &drawer = MonoGlyph_.drawer();
  Size size = MonoGlyph_.terminal().size();
  Glyph &rightGlyph = MonoGlyph_.rightLetter();

  ScreenBuffer letter((size.x - 20) / 4.5f, (size.y - 6) / 3);
  Drawer letterDrawer(letter);

  letterDrawer.drawView(0, 0, rightGlyph.segments, L'-');

  drawer.drawBuffer(size.x - (size.x - 20) / 4.5f -
                        (size.x - (size.x - 20) / 4.5f) / 6,
                    (size.y - (size.y - 6) / 3) / 2, letter);
}
