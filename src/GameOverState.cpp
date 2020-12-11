//
// Created by Domagoj on 06/11/2020.
//


#include "GameOverState.h"
#include "GameState.h"
#include "EnterHighscoreState.h"
#include "Highscore/Highscore.h"

GameOverState *GameOverState::instance = nullptr;

GameOverState::GameOverState(uint _score){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();

	score = _score;

	instance = this;

	melodyTime = Melody.playMelody(LOSE, false);
}

void GameOverState::loop(uint){

	states();

	gameOverMessage();


	if(millis() - gameOverMillis > melodyTime){

		if(Highscore.dataCount() > 3 || score > Highscore.get(2).score)
			game->changeState(new EnterHighscoreState(score));
	}

	display->commit();
}

void GameOverState::enter(Game &_game){

	game = &_game;

	pointer = {30, 68, 70, 20};

	Input::getInstance()->setBtnPressCallback(BTN_UP, buttonUpPressed);
	Input::getInstance()->setBtnPressCallback(BTN_DOWN, buttonDownPressed);
	Input::getInstance()->setBtnPressCallback(BTN_A, buttonAPressed);

	Input::getInstance()->setBtnReleaseCallback(BTN_UP, buttonUpReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_DOWN, buttonDownReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_A, buttonAReleased);

}

void GameOverState::exit(){

	score = 0;

	Input::getInstance()->removeBtnPressCallback(BTN_UP);
	Input::getInstance()->removeBtnReleaseCallback(BTN_UP);
	Input::getInstance()->removeBtnPressCallback(BTN_DOWN);
	Input::getInstance()->removeBtnReleaseCallback(BTN_DOWN);
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnReleaseCallback(BTN_A);

}

void GameOverState::states(){

	if(upState)
		pointer = {30, 68, 70, 20};
	else if(downState)
		pointer = {25, 88, 80, 20};

}


void GameOverState::gameOverMessage(){

	baseSprite->clear(TFT_BLACK);
	baseSprite->setTextSize(1);
	baseSprite->setTextFont(2);
	baseSprite->setTextColor(TFT_WHITE);
	baseSprite->drawString(endMessage, 35, 10);
	baseSprite->drawString(finalScore, 35, 30);
	baseSprite->drawNumber(score, 85, 30);

}

void GameOverState::buttonUpPressed(){

	instance->upState = true;
}

void GameOverState::buttonDownPressed(){

	instance->downState = true;
}

void GameOverState::buttonAPressed(){

	instance->aState = true;
}

void GameOverState::buttonUpReleased(){

	instance->upState = false;
}

void GameOverState::buttonDownReleased(){

	instance->downState = false;
}

void GameOverState::buttonAReleased(){

	instance->aState = false;
}

