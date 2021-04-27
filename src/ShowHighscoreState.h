#ifndef NIBBLE_NEW_SHOWHIGHSCORESTATE_H
#define NIBBLE_NEW_SHOWHIGHSCORESTATE_H

#include "Game.h"
#include "State.h"
#include "Menu.h"

namespace TroublesomeSpace {

	class ShowHighscoreState : public State {

	public:

		ShowHighscoreState(Display &display);

		void enter(Game &game) override;

		void exit() override;

		void loop(uint) override;

		void draw();

		static void buttonBPressed();

		static void buttonBReleased();

	private:

		static ShowHighscoreState *instance;

		Menu *menu;

		bool bState;

		uint previousArrowTime = 0;
		int step = 0;

		const char *highscore = "Highscore:";
		const char *back = "B";

	};

}

#endif //NIBBLE_NEW_SHOWHIGHSCORESTATE_H
