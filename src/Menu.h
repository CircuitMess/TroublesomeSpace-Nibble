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

	static void buttonUpPressed();
	static void buttonDownPressed();
	static void buttonAPressed();
	static void buttonUpReleased();
	static void buttonDownReleased();
	static void buttonAReleased();

private:

	static Menu *instance;

	Melody *melody;

	void drawMenu();
	void states();

	rectPointer pointer = {30, 70, 70, 20};

	bool upState = false;
	bool downState = false;
	bool aState = false;

	uint melodyTime = 0;

	const char *title = "The Hardest Game";
	const char *newGame = "New Game";
	const char *highScore = "High Score";

};


#endif //NIBBLE_NEW_MENU_H
