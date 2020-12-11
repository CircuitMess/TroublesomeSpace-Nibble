//
// Created by Domagoj on 02/12/2020.
//

#ifndef NIBBLE_NEW_ERASEHIGHSCORESTATE_H
#define NIBBLE_NEW_ERASEHIGHSCORESTATE_H

#include <Arduino.h>
#include "Game.h"

class EraseHighscoreState : public State {

public:

	EraseHighscoreState();

	void enter(Game& game) override;

	void exit() override;

	void loop(uint) override;

	void draw();

	static void buttonAPressed();
	static void buttonBPressed();
	static void buttonAReleased();
	static void buttonBReleased();

private:

	Display *display;
	Sprite *baseSprite;

	Game *game;

	static EraseHighscoreState *instance;

	bool aState;
	bool bState;

	const char *check = "Are you sure ?";
	const char *yes = "A -> Yes";
	const char *no = "B -> No";

};


#endif //NIBBLE_NEW_ERASEHIGHSCORESTATE_H
