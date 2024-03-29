#ifndef NIBBLE_NEW_ENTERHIGHSCORESTATE_H
#define NIBBLE_NEW_ENTERHIGHSCORESTATE_H

#include <Arduino.h>
#include "Game.h"
#include "State.h"

namespace TroublesomeSpace {

	class EnterHighscoreState : public State {

	public:

		explicit EnterHighscoreState(Display &display, uint score);

		void enter(Game &game) override;

		void exit() override;

		void loop(uint) override;

		void draw();

		void states();

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

		static EnterHighscoreState *instance;

		Display *display;
		Sprite *baseSprite;

		Game *game;

		uint score = 0;
		char *name;
		const char *dispName = "Name:";

		uint8_t charCursor;

		uint32_t previousCursorTime = millis();

		bool upState = false;
		bool downState = false;
		bool aState = false;
		bool bState = false;
		bool leftState = false;
		bool rightState = false;

	};

}

#endif //NIBBLE_NEW_ENTERHIGHSCORESTATE_H
