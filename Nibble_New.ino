#include <Arduino.h>
#include <CircuitOS.h>
#include "src/Game.h"
#include <Input/Input.h>
#include <Input/I2cExpander.h>
#include <Input/InputI2C.h>
#include <Nibble.h>
#include <Loop/LoopManager.h>
#include <gpio.h>
#include "src/melody/Melody.h"

Game *game;
Melody *melody;

void setup(){

	Nibble.begin();

	melody = new Melody;
	game = new Game(melody);

	LoopManager::addListener(melody);
	LoopManager::addListener(game);

	LoopManager::addListener(Input::getInstance());

}

void loop(){

	LoopManager::loop();
}
