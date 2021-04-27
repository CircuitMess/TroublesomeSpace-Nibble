#include <Arduino.h>
#include <LittleFS.h>
#include <CircuitOS.h>
#include <Nibble.h>
#include <Loop/LoopManager.h>
#include "src/Game.h"
#include "src/Highscore/Highscore.h"
//#include "GameInfo.hpp"

TroublesomeSpace::Game *game;
Display display(128,128,-1,1);

void setup(){

	Nibble.begin();
	display.begin();

	Serial.begin(115200);
	Piezo.setMute(true);

	TroublesomeSpace::Highscore.begin();
	game = new TroublesomeSpace::Game(display);

	LoopManager::addListener(game);
	LoopManager::addListener(Input::getInstance());
}

void loop(){

	LoopManager::loop();
}
