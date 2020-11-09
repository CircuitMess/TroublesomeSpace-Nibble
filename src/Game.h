//
// Created by Domagoj on 06/11/2020.
//

#ifndef NIBBLE_NEW_GAME_H
#define NIBBLE_NEW_GAME_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Nibble.h>
#include <Loop/LoopManager.h>
#include "melody/Melody.h"

class State;

class Game : public LoopListener {

public:

	Game();

	void loop(uint) override;

	void changeState(State *s);

private:

	State *state{};
	Melody *melody;

};


#endif //NIBBLE_NEW_GAME_H
