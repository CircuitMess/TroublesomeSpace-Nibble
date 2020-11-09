#include <climits>
//
// Created by Domagoj on 06/11/2020.
//

#ifndef NIBBLE_NEW_GAMEOVERSTATE_H
#define NIBBLE_NEW_GAMEOVERSTATE_H


#include "State.h"
#include "melody/Melody.h"

class GameState;

class GameOverState : public State {

public:

	GameOverState(gameOverType type, GameState *gameState1, Melody *mel);

	~GameOverState() override;

	void loop(uint) override;

	void enter(Game& game) override;

	void exit() override;


	//static void buttonUpPressed();
	//static void buttonDownPressed();

	//static void buttonAPressed();

	//static void buttonUpReleased();
	//static void buttonDownReleased();

	//static void buttonAReleased();

private:

	static GameOverState *instance;

	GameState *gameState;

	Melody *melody;

	//void victory();

	//void gameOver();

	void victoryMessage();

	void gameOverMessage();

	const char *winMessage = "Victory!";
	const char *finalScore = "Score: ";
	const char *endMessage = "GameState Over";

	bool aState = false;

	bool victory = false;
	bool gameOver = false;
	bool gameOverPlaying = false;
	bool victoryPlaying = false;
	unsigned long gameOverMillis = 0;
	unsigned long victoryMillis = 0;
};


#endif //NIBBLE_NEW_GAMEOVERSTATE_H
