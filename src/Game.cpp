//
// Created by Domagoj on 06/11/2020.
//

#include "Game.h"
#include "GameState.h"
#include "GameOverState.h"
#include "Menu.h"

Game::Game(){

	melody = new Melody;
	LoopManager::addListener(melody);

	state = new Menu(melody);

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

