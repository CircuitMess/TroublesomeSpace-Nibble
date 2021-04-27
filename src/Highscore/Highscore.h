#ifndef NIBBLE_NEW_HIGHSCORE_H
#define NIBBLE_NEW_HIGHSCORE_H

#include <Arduino.h>

#define MAX_SCORE_PLAYERS 5
#define MAX_PLAYER_LETTERS 5
#define FILENAME "/TroublesomeSpace"

namespace TroublesomeSpace {

	struct Score {

		char name[MAX_PLAYER_LETTERS + 1];
		uint score;
	};

	class HighscoreImpl {

	public:

		HighscoreImpl();

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

	extern HighscoreImpl Highscore;

}

#endif //NIBBLE_NEW_HIGHSCORE_H
