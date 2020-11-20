//
// Created by Domagoj on 13/11/2020.
//

#ifndef NIBBLE_NEW_HIGHSCORE_H
#define NIBBLE_NEW_HIGHSCORE_H

#include "Game.h"
#include "State.h"
#include "Menu.h"

#define MAX_SCORE_PLAYERS 5
#define MAX_PLAYER_LETTERS 5
#define FILENAME "/THG"

struct Score {

	char name[MAX_PLAYER_LETTERS + 1];
	uint score;
};

class Highscore : public State {

public:

	Highscore(int score, bool previousState, Melody *melody);

	void enter(Game &game) override;

	void exit() override;

	void loop(uint) override;

	void begin();

	void addData(Score &score);

	void clearData();

	const Score& get(uint8_t i);

	uint8_t dataCount();

	static void buttonUpPressed();

	static void buttonDownPressed();

	static void buttonAPressed();

	static void buttonBPressed();

	static void buttonUpReleased();

	static void buttonDownReleased();

	static void buttonAReleased();

	static void buttonBReleased();

private:

	static Highscore *instance;

	Menu *menu;
	Melody *melody;
	uint melodyTime = 0;

	void states();

	void draw();

	struct Data {
		uint8_t count;
		Score scores[MAX_SCORE_PLAYERS];
	} data;

	void saveData();
	void loadData();

	bool upState;
	bool downState;
	bool aState;
	bool bState;

	Score score;
	bool previousState; // 0-menu ; 1-game over

	const char *highscore = "Highscore";
	const char *backMessage = "B: Back";
	const char *confirfmMessage = "A: Accept";
	const char *deleteMessage = "A: Delete";

};


#endif //NIBBLE_NEW_HIGHSCORE_H
