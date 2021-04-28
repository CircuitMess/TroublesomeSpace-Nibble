#ifndef GAME_INFO
#define GAME_INFO
class Context;
struct GameInfo
{
	const char *title;
	const char *description;
	const uint16_t *icon;
	Context *(*launch)(Display &display);
};
#endif

#include <Arduino.h>
#include <Display/Display.h>
#include "src/bitmaps/launcherIcon/TS_icon.hpp"
#include "src/Game.h"
const GameInfo TroublesomeSpaceInfo
		{
				"TroublesomeSpace",
				"Explore the Space and avoid troubles along the way!",
				TS_icon,
				[](Display &display) -> Context * { return new TroublesomeSpace::Game(display); }
		};
