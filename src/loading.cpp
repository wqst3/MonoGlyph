#include "MonoGlyph.h"

MonoGlyph::State MonoGlyph::handleLoading()
{
	return State::Menu;
}

void MonoGlyph::drawLoadingFrame(bool next)
{

}

bool MonoGlyph::loadingDone()
{
	return true;
}

