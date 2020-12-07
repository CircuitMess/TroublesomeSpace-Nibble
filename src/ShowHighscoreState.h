//
// Created by Domagoj on 13/11/2020.
//

#ifndef NIBBLE_NEW_SHOWHIGHSCORESTATE_H
#define NIBBLE_NEW_SHOWHIGHSCORESTATE_H

#include "Game.h"
#include "State.h"
#include "Menu.h"
#include "Highscore/Highscore.h"

#define MAX_SCORE_PLAYERS 5
#define MAX_PLAYER_LETTERS 5
#define FILENAME "/THG"


class ShowHighscoreState : public State {

public:

	ShowHighscoreState();

	void enter(Game &game) override;

	void exit() override;

	void loop(uint) override;

	void draw();

	static void buttonAPressed();
	static void buttonBPressed();
	static void buttonAReleased();
	static void buttonBReleased();

private:

	static ShowHighscoreState *instance;

	Menu *menu;

	Highscore hs;

	bool aState;
	bool bState;

	const char *highscore = "Highscore:";
	const char *backMessage = "B: Menu";
	const char *deleteMessage = "A: Delete";

};


#endif //NIBBLE_NEW_SHOWHIGHSCORESTATE_H
