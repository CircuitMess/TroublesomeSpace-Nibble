//
// Created by Domagoj on 04/12/2020.
//

#ifndef NIBBLE_NEW_HIGHSCORE_H
#define NIBBLE_NEW_HIGHSCORE_H

#include <Arduino.h>

#define MAX_SCORE_PLAYERS 5
#define MAX_PLAYER_LETTERS 5
#define FILENAME "/TheGame"

struct Score {

	char name[MAX_PLAYER_LETTERS + 1];
	uint score;
};

class Highscore {

public:

	Highscore();

	void begin();

	void addData(Score &score);

	void clearData();

	const Score &get(uint8_t i);

	uint8_t dataCount();

private:

	struct Data {
		uint8_t count;
		Score scores[MAX_SCORE_PLAYERS];
	} data;

	void saveData();

	void loadData();

};



#endif //NIBBLE_NEW_HIGHSCORE_H
