//
// Created by Domagoj on 06/11/2020.
//

#ifndef NIBBLE_NEW_STATE_H
#define NIBBLE_NEW_STATE_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Nibble.h>
#include <Loop/LoopManager.h>
#include <Display/Display.h>
#include <Display/Sprite.h>

class Game;

class State {

public:

	State();

	virtual void enter(Game& game) = 0;

	virtual void exit() = 0;

	virtual void loop(uint) = 0;


protected:

	Game *game;
	Display *display;
	Sprite *baseSprite;

	enum gameOverType{
		W,				//victory
		L				// game over
	};

};

#endif //NIBBLE_NEW_STATE_H
