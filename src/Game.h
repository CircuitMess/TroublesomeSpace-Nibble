#ifndef NIBBLE_NEW_GAME_H
#define NIBBLE_NEW_GAME_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Nibble.h>
#include <Loop/LoopManager.h>

namespace TroublesomeSpace {

	class State;

	class Game : public LoopListener {

	public:

		Game(Display &display);

		void loop(uint) override;

		void changeState(State *s);

	private:

		State *state{};
	};

}

#endif //NIBBLE_NEW_GAME_H
