#pragma once

#include "stateID.h"

#include <memory>

class MonoGlyph;

class State
{

public:
	virtual ~State() = default;

	virtual void onEnter(MonoGlyph&) {}
	virtual void onEvent(MonoGlyph&) = 0;
	virtual StateID id() const = 0;
};

