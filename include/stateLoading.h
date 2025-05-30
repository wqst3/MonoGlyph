#pragma once

#include "state.h"

class StateLoading : public State {
  void draw(MonoGlyph &);
  void input(MonoGlyph &, char);

 public:
  void onEnter(MonoGlyph &) override;
  void onEvent(MonoGlyph &) override;
  StateID id() const override { return StateID::Menu; }
};
