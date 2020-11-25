#include <Arduino.h>
#include <CircuitOS.h>
#include <Input/Input.h>
#include <Input/I2cExpander.h>
#include <Input/InputI2C.h>
#include <Nibble.h>
#include <Loop/LoopManager.h>
#include <gpio.h>
#include "src/melody/Melody.h"
#include "src/Game.h"
#include "src/Highscore.h"

Game *game;

void setup(){

	Nibble.begin();
	Serial.begin(115200);
	Piezo.setMute(false);

	game = new Game;

	LoopManager::addListener(game);
	LoopManager::addListener(Input::getInstance());
}

void loop(){

	LoopManager::loop();
}
