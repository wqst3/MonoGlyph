#pragma once

#include "state.h"

class StateRestart : public State {
  void draw(MonoGlyph &);
  void input(MonoGlyph &, char);

 public:
  void onEvent(MonoGlyph &) override;
  StateID id() const override { return StateID::Restart; }
};
