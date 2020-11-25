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

	Highscore(uint score, bool previousState);

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

	static Highscore *instance;

	Menu *menu;

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
	bool leftState;
	bool rightState;

	Score _score;
	bool previousState; // 0-menu ; 1-game over
	bool addScore = true;

	char* name;
	uint8_t charCursor;

	const char *highscore = "Highscore";
	const char *backMessage = "B: Back";
	const char *confirfmMessage = "A: Accept";
	const char *deleteMessage = "A: Delete";

};

extern Highscore HS;

#endif //NIBBLE_NEW_HIGHSCORE_H
