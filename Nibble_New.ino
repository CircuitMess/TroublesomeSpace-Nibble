#include <Arduino.h>
#include <LittleFS.h>
#include <CircuitOS.h>
#include <Nibble.h>
#include <Loop/LoopManager.h>
#include "src/Game.h"
#include "src/Highscore/Highscore.h"

Game *game;

void setup(){

	Nibble.begin();
	Serial.begin(115200);
	Piezo.setMute(true);

	Highscore.begin();
	game = new Game;

	LoopManager::addListener(game);
	LoopManager::addListener(Input::getInstance());
}

void loop(){

	LoopManager::loop();
}
