//
// Created by Domagoj on 11/11/2020.
//

#ifndef NIBBLE_NEW_MENU_H
#define NIBBLE_NEW_MENU_H

#include "Game.h"
#include "State.h"


class Menu : public State {

public:

	Menu();

	void enter(Game &game) override;
	void exit() override;
	void loop(uint) override;
	void draw();

	static void buttonUpPressed();
	static void buttonDownPressed();
	static void buttonAPressed();
	static void buttonUpReleased();
	static void buttonDownReleased();
	static void buttonAReleased();

private:

	static Menu *instance;

	void states();
	void drawTriangleArrows();

	bool upState;
	bool downState;
	bool aState;

	uint melodyTime = 0;

	uint previousArrowTime = 0;
	int step = 0;

	int menuOrder = 0;

	const char *newGame = "New Game";
	const char *highScore = "High Score";
	const char *eraseHighscore = "Erase High Score";
	const char *info = "Info";
};


#endif //NIBBLE_NEW_MENU_H
