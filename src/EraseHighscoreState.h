#ifndef NIBBLE_NEW_ERASEHIGHSCORESTATE_H
#define NIBBLE_NEW_ERASEHIGHSCORESTATE_H

#include <Arduino.h>
#include "Game.h"

namespace TroublesomeSpace {

	class EraseHighscoreState : public State {

	public:

		EraseHighscoreState(Display &display);

		void enter(Game &game) override;

		void exit() override;

		void loop(uint) override;

		void draw();

		static void buttonAPressed();

		static void buttonBPressed();

		static void buttonAReleased();

		static void buttonBReleased();

	private:

		static EraseHighscoreState *instance;

		bool aState;
		bool bState;

		const char *confirm = "CONFIRM";
		const char *yes = "A: yes";
		const char *no = "B: no";

	};

}

#endif //NIBBLE_NEW_ERASEHIGHSCORESTATE_H
