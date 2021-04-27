#ifndef NIBBLE_NEW_GAMEINFOSTATE_H
#define NIBBLE_NEW_GAMEINFOSTATE_H

#include "Game.h"
#include "State.h"

namespace TroublesomeSpace {

	class GameInfoState : public State {

	public:
		GameInfoState(Display &display);

		~GameInfoState();

		void enter(Game &game) override;

		void exit() override;

		void loop(uint) override;

		void draw();


		static void buttonAPressed();

		static void buttonBPressed();

		static void buttonAReleased();

		static void buttonBReleased();

	private:

		static GameInfoState *instance;

		void states();

		bool aState = false;
		bool bState = false;

		int step = 0;
		uint previousArrowTime = 0;

		bool invisible = false;
	};

}

#endif //NIBBLE_NEW_GAMEINFOSTATE_H
