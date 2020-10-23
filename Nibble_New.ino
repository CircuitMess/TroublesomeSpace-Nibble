#include <Arduino.h>
#include <CircuitOS.h>
#include "src/Game.h"
#include <Input/Input.h>
#include <Input/I2cExpander.h>
#include <Input/InputI2C.h>
#include <Nibble.h>
#include <Loop/LoopManager.h>
#include <spiffs_api.h>
#include <gpio.h>

Game *game;

void setup(){

	Serial.begin(115200);
	Nibble.begin();

	game = new Game;

	LoopManager::addListener(game);
	LoopManager::addListener(Input::getInstance());

}

void loop(){

	LoopManager::loop();
}
