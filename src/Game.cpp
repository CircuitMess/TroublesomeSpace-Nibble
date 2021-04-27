#include "Game.h"
#include "GameState.h"
#include "GameOverState.h"
#include "Melody/Melody.h"

TroublesomeSpace::Game::Game(Display &display){

	LoopManager::addListener(&Melody);
	state = new Menu(display);
	state->enter(*this);
}

void TroublesomeSpace::Game::loop(uint t){

	state->loop(t);
}

void TroublesomeSpace::Game::changeState(State *s){

	state->exit();
	delete state;
	state = s;
	state->enter(*this);

}

