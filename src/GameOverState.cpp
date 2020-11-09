//
// Created by Domagoj on 06/11/2020.
//


#include "GameOverState.h"
#include "GameState.h"

GameOverState *GameOverState::instance = nullptr;

GameOverState::GameOverState(gameOverType type, GameState *gameState1, Melody *mel){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();

	melody = mel;
	gameState = gameState1;

	instance = this;

	switch(type){

		case State::W:
			victory = true;
			break;

		case State::L:
			gameOver = true;
			break;

		default:
			return;

	}

	gameState->score = 0;
	gameState->lives = 3;
	gameState->invisibilityCounter = 3;


	if(victory){
		victoryPlaying = true;
		gameState->loopPlaying = false;
		victoryMillis = millis();
		gameState->melodyTime = melody->playMelody(VICTORY, false);
	}else if(gameOver){
		gameOverPlaying = true;
		gameState->loopPlaying = false;
		gameOverMillis = millis();
		gameState->melodyTime = melody->playMelody(LOSE, false);
	}

}

GameOverState::~GameOverState(){
	exit();
}

void GameOverState::loop(uint){

	if(gameOverPlaying){

		gameOverMessage();

		if(millis() - gameOverMillis > gameState->melodyTime){
			gameOverPlaying = false;
			gameState->loopPlaying = true;

			game->changeState(new GameState(melody));
		}
	}

	if(victoryPlaying){

		victoryMessage();

		if(millis() - victoryMillis > gameState->melodyTime){
			victoryPlaying = false;
			gameState->loopPlaying = true;

			game->changeState(new GameState(melody));
		}
	}
}

void GameOverState::enter(Game& _game){

	game = &_game;

	//Input::getInstance()->setBtnPressCallback(BTN_A, buttonAPressed);
	//Input::getInstance()->setBtnReleaseCallback(BTN_A, buttonAReleased);

}

void GameOverState::exit(){

	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnReleaseCallback(BTN_A);

}
/*
void GameOverState::victory(){

	for(int i = 0; i < score / 10; i++)
		triangles.pop_back();

	score = 0;
	lives = 3;
	invisibilityCounter = 3;

	victoryPlaying = true;
	loopPlaying = false;
	victoryMillis = millis();
	melodyTime = melody->playMelody(VICTORY, false);

}
*/
void GameOverState::victoryMessage(){

	baseSprite->clear(TFT_BLACK);
	baseSprite->setTextSize(1);
	baseSprite->setTextFont(2);
	baseSprite->setTextColor(TFT_WHITE);
	baseSprite->drawString(winMessage, 35, 55);
	display->commit();

}
/*
void GameOverState::gameOver(){

	for(int i = 0; i < score / 10; i++)
		triangles.pop_back();

	score = 0;
	lives = 3;
	invisibilityCounter = 3;

	gameOverPlaying = true;
	loopPlaying = false;
	gameOverMillis = millis();
	melodyTime = melody->playMelody(LOSE, false);

}
*/
void GameOverState::gameOverMessage(){

	baseSprite->clear(TFT_BLACK);
	baseSprite->setTextSize(1);
	baseSprite->setTextFont(2);
	baseSprite->setTextColor(TFT_WHITE);
	baseSprite->drawString(endMessage, 35, 50);
	baseSprite->drawString(finalScore, 40, 80);
	baseSprite->drawNumber(gameState->score, 90, 80);
	display->commit();
}
/*
void GameOverState::buttonAPressed(){

	instance->aState = true;
}

void GameOverState::buttonAReleased(){

	instance->aState = false;
}
 */