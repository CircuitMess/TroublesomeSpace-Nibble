#ifndef NIBBLE_NEW_GAMEOVERSTATE_H
#define NIBBLE_NEW_GAMEOVERSTATE_H


#include "State.h"
#include "Menu.h"

namespace TroublesomeSpace {

	class GameState;

	class GameOverState : public State {

	public:

		GameOverState(Display &display, uint score);

		void loop(uint) override;

		void enter(Game &game) override;

		void exit() override;

		void draw();

	private:

		unsigned long gameOverMillis = millis();

		uint melodyTime;

		uint score = 0;

	};

}

#endif //NIBBLE_NEW_GAMEOVERSTATE_H
