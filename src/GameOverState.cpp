//
// Created by Domagoj on 06/11/2020.
//


#include "GameOverState.h"
#include "GameState.h"

GameOverState *GameOverState::instance = nullptr;

GameOverState::GameOverState(gameOverType type, Melody *mel, int _score){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();

	melody = mel;
	score = _score;

	instance = this;

	switch(type){

		case State::W:
			victory = true;
			melodyTime = melody->playMelody(VICTORY, false);
			break;

		case State::L:
			gameOverMessage();
			//newGameOption();
			display->commit();
			Serial.println("case lose passed");
			gameOver = true;
			melodyTime = melody->playMelody(LOSE, false);
			Serial.println("end case passed");
			break;

		default:
			return;

	}

}

void GameOverState::loop(uint){

	if(gameOver){
		Serial.println("if(gameover) passed");
		gameOverMessage();
		newGameOption();
		Serial.println("message display passed");
		if(millis() - gameOverMillis > melodyTime){
			Serial.println("melody time passed");
			if(instance->aState)
				game->changeState(new GameState(melody));
		}


	}else if(victory){

		victoryMessage();
		newGameOption();

		if(millis() - victoryMillis > melodyTime){

			if(instance->aState)
				game->changeState(new GameState(melody));
		}


	}
	display->commit();
}

void GameOverState::enter(Game &_game){

	game = &_game;

	Input::getInstance()->setBtnPressCallback(BTN_UP, buttonUpPressed);
	Input::getInstance()->setBtnPressCallback(BTN_DOWN, buttonDownPressed);
	Input::getInstance()->setBtnPressCallback(BTN_A, buttonAPressed);

	Input::getInstance()->setBtnReleaseCallback(BTN_UP, buttonUpReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_DOWN, buttonDownReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_A, buttonAReleased);

}

void GameOverState::exit(){

	victory = false;
	gameOver = false;

	Input::getInstance()->removeBtnPressCallback(BTN_UP);
	Input::getInstance()->removeBtnReleaseCallback(BTN_UP);
	Input::getInstance()->removeBtnPressCallback(BTN_DOWN);
	Input::getInstance()->removeBtnReleaseCallback(BTN_DOWN);
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnReleaseCallback(BTN_A);

}

void GameOverState::victoryMessage(){

	baseSprite->clear(TFT_BLACK);
	baseSprite->setTextSize(1);
	baseSprite->setTextFont(2);
	baseSprite->setTextColor(TFT_WHITE);
	baseSprite->drawString(winMessage, 35, 55);


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

void GameOverState::newGameOption(){

	baseSprite->drawString(newGame, 35, 70);

	if(instance->downState == 1){
		baseSprite->drawRect(30, 65, 70, 30, TFT_GREEN);
	}else if(instance->upState == 1){

	}
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

