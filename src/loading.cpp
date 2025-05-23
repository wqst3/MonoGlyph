#include "MonoGlyph.h"

MonoGlyph::State MonoGlyph::handleLoading()
{
	eventLoop(
		[&](){ drawLoadingFrame(true); },
		[&](){ onResize(); drawLoadingFrame(false); },
		[&](char ch){ },
		[&](){ return loadingDone(); }
	);
	if (loadThread_.joinable()) loadThread_.join();

	return State::Menu;
}

void MonoGlyph::drawLoadingFrame(bool next)
{

}

bool MonoGlyph::loadingDone()
{
	return fontsLoaded_.load();
}

