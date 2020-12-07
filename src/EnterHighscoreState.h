//
// Created by Domagoj on 02/12/2020.
//

#ifndef NIBBLE_NEW_ENTERHIGHSCORESTATE_H
#define NIBBLE_NEW_ENTERHIGHSCORESTATE_H

#include <Arduino.h>
#include "Game.h"
#include "State.h"
#include "Highscore/Highscore.h"

class EnterHighscoreState : public State {

public:

	explicit EnterHighscoreState(uint score);

	void enter(Game& game) override;

	void exit() override;

	void loop(uint) override;

	void draw();

	void states();

	static void buttonUpPressed();

	static void buttonDownPressed();

	static void buttonLeftPressed();

	static void buttonRightPressed();

	static void buttonAPressed();

	static void buttonBPressed();

	static void buttonUpReleased();

	static void buttonDownReleased();

	static void buttonLeftReleased();

	static void buttonRightReleased();

	static void buttonAReleased();

	static void buttonBReleased();

private:

	static EnterHighscoreState *instance;

	Display *display;
	Sprite *baseSprite;

	Game *game;

	Highscore hs;

	uint score = 0;
	char *name;

	uint8_t charCursor;

	bool upState;
	bool downState;
	bool aState;
	bool bState;
	bool leftState;
	bool rightState;

};


#endif //NIBBLE_NEW_ENTERHIGHSCORESTATE_H
