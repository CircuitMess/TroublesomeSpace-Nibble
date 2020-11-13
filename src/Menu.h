//
// Created by Domagoj on 11/11/2020.
//

#ifndef NIBBLE_NEW_MENU_H
#define NIBBLE_NEW_MENU_H

#include "Game.h"
#include "State.h"

struct rectPointer{

	int x;
	int y;
	int width;
	int height;

};

class Menu : public State {

public:

	explicit Menu(Melody *melody);

	void enter(Game &game) override;
	void exit() override;
	void loop(uint) override;
	void drawMenu();

	static void buttonUpPressed();
	static void buttonDownPressed();
	static void buttonAPressed();
	static void buttonUpReleased();
	static void buttonDownReleased();
	static void buttonAReleased();

private:

	static Menu *instance;

	Melody *melody;

	void states();

	rectPointer pointer = {30, 68, 70, 20};

	bool upState;
	bool downState;
	bool aState;

	uint melodyTime = 0;

	const char *title = "The Hardest Game";
	const char *newGame = "New Game";
	const char *highScore = "High Score";
};


#endif //NIBBLE_NEW_MENU_H
