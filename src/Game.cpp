#include "Game.h"
#include "GameState.h"
#include "GameOverState.h"

Game::Game(){

	LoopManager::addListener(&Melody);
	Serial.println("test");
	state = new Menu();
	state->enter(*this);
}

void Game::loop(uint t){

	state->loop(t);
}

void Game::changeState(State *s){

	state->exit();
	delete state;
	state = s;
	state->enter(*this);

}

