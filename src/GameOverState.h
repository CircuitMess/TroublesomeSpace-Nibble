//
// Created by Domagoj on 06/11/2020.
//

#ifndef NIBBLE_NEW_GAMEOVERSTATE_H
#define NIBBLE_NEW_GAMEOVERSTATE_H


#include "State.h"
#include "Menu.h"
#include "melody/Melody.h"

class GameState;

class GameOverState : public State {

public:

	GameOverState(gameOverType type, uint score, bool prevState);

	void loop(uint) override;

	void enter(Game& game) override;

	void exit() override;

	static void buttonUpPressed();
	static void buttonDownPressed();
	static void buttonAPressed();

	static void buttonUpReleased();
	static void buttonDownReleased();
	static void buttonAReleased();

private:

	static GameOverState *instance;

	rectPointer pointer;

	void states();

	void drawGameOver();

	void victoryMessage();

	void gameOverMessage();

	const char *winMessage = "Victory!";
	const char *finalScore = "Score: ";
	const char *endMessage = "Game Over";
	const char *newGame = "New Game";
	const char *highScore = "High Score";

	bool aState = false;
	bool upState = false;
	bool downState = false;

	bool victory = false;
	bool gameOver = false;

	unsigned long gameOverMillis = 0;
	unsigned long victoryMillis = 0;

	uint melodyTime;

	uint score = 0;
	bool previousState; // 0-previous state was gameState
						// 1-previous state was highscoreState
};


#endif //NIBBLE_NEW_GAMEOVERSTATE_H
