#pragma once

#include "state.h"

class StateInfinite : public State {
  void draw(MonoGlyph &);
  void input(MonoGlyph &, char);

 public:
  void onEvent(MonoGlyph &) override;
  StateID id() const override { return StateID::Infinite; }
};
