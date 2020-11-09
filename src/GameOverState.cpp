//
// Created by Domagoj on 06/11/2020.
//


#include "GameOverState.h"
#include "GameState.h"

GameOverState *GameOverState::instance = nullptr;

GameOverState::GameOverState(gameOverType type, Melody *mel, int _score, bool _loopPlaying, uint _melodyTime){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();

	melody = mel;
	score = _score;
	loopPlaying = _loopPlaying;
	melodyTime = _melodyTime;

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


	if(victory){
		victoryPlaying = true;
		loopPlaying = false;
		victoryMillis = millis();
		melodyTime = melody->playMelody(VICTORY, false);
	}else if(gameOver){
		gameOverPlaying = true;
		loopPlaying = false;
		gameOverMillis = millis();
		melodyTime = melody->playMelody(LOSE, false);
	}

}

void GameOverState::loop(uint){

	if(gameOverPlaying){

		gameOverMessage();

		if(millis() - gameOverMillis > melodyTime){
			gameOverPlaying = false;
			//loopPlaying = true;

			game->changeState(new GameState(melody));
		}
	}

	if(victoryPlaying){

		victoryMessage();

		if(millis() - victoryMillis > melodyTime){
			victoryPlaying = false;
			//loopPlaying = true;

			game->changeState(new GameState(melody));
		}
	}
}

void GameOverState::enter(Game &_game){

	game = &_game;

	//Input::getInstance()->setBtnPressCallback(BTN_A, buttonAPressed);
	//Input::getInstance()->setBtnReleaseCallback(BTN_A, buttonAReleased);

}

void GameOverState::exit(){

	//Input::getInstance()->removeBtnPressCallback(BTN_A);
	//Input::getInstance()->removeBtnReleaseCallback(BTN_A);

}

void GameOverState::victoryMessage(){

	baseSprite->clear(TFT_BLACK);
	baseSprite->setTextSize(1);
	baseSprite->setTextFont(2);
	baseSprite->setTextColor(TFT_WHITE);
	baseSprite->drawString(winMessage, 35, 55);
	display->commit();

}

void GameOverState::gameOverMessage(){

	baseSprite->clear(TFT_BLACK);
	baseSprite->setTextSize(1);
	baseSprite->setTextFont(2);
	baseSprite->setTextColor(TFT_WHITE);
	baseSprite->drawString(endMessage, 35, 50);
	baseSprite->drawString(finalScore, 40, 80);
	baseSprite->drawNumber(score, 90, 80);
	display->commit();
}
