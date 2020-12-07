#include <Arduino.h>
#include <CircuitOS.h>
#include <Nibble.h>
#include <Loop/LoopManager.h>
#include "src/Game.h"

Game *game;

void setup(){

	Nibble.begin();
	Serial.begin(115200);
	Piezo.setMute(true);

	game = new Game;

	LoopManager::addListener(game);
	LoopManager::addListener(Input::getInstance());
}

void loop(){

	LoopManager::loop();
}
