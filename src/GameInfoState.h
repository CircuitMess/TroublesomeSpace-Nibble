#ifndef NIBBLE_NEW_GAMEINFOSTATE_H
#define NIBBLE_NEW_GAMEINFOSTATE_H

#include "Game.h"
#include "State.h"

class GameInfoState : public State {

public:
	GameInfoState();
	~GameInfoState();

	void enter(Game &game) override;
	void exit() override;
	void loop(uint) override;
	void draw();

	static void buttonUpPressed();
	static void buttonDownPressed();
	static void buttonAPressed();
	static void buttonBPressed();
	static void buttonUpReleased();
	static void buttonDownReleased();
	static void buttonAReleased();
	static void buttonBReleased();

private:

	static GameInfoState *instance;

	void states();

	bool upState;
	bool downState;
	bool aState;
	bool bState;

	int step = 0;
	uint previousArrowTime = 0;

	bool invisible = false;
};


#endif //NIBBLE_NEW_GAMEINFOSTATE_H
