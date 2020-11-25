//
// Created by Domagoj on 06/11/2020.
//


#include "GameOverState.h"
#include "GameState.h"
#include "Highscore.h"

GameOverState *GameOverState::instance = nullptr;

GameOverState::GameOverState(gameOverType type, uint _score, bool prevState){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();

	score = _score;
	previousState = prevState;

	instance = this;

	switch(type){

		case State::W:
			victory = true;
			if(previousState)
				break;
			else
				melodyTime = Melody.playMelody(VICTORY, false);
			break;

		case State::L:
			gameOver = true;
			if(previousState)
				break;
			else
				melodyTime = Melody.playMelody(LOSE, false);
			break;

		default:
			return;

	}

}

void GameOverState::loop(uint){

	states();

	if(gameOver){

		gameOverMessage();


		if(millis() - gameOverMillis > melodyTime){

			if(!previousState){
				game->changeState(new Highscore(score, true));
				previousState = true;
			}else{
				drawGameOver();
				if(aState && pointer.y == 68)
					game->changeState(new GameState());
				if(aState && pointer.y == 88)
					game->changeState(new Highscore(score,true));

			}
		}


	}else if(victory){

		victoryMessage();
		drawGameOver();

		if(millis() - victoryMillis > melodyTime){

			if(aState && pointer.y == 68)
				game->changeState(new GameState());
			if(aState && pointer.y == 88)
				game->changeState(new Highscore(score, true));
		}


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

	victory = false;
	gameOver = false;
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

void GameOverState::drawGameOver(){


	baseSprite->setTextSize(1);
	baseSprite->setTextFont(2);
	baseSprite->setTextColor(TFT_WHITE);
	baseSprite->drawString(newGame, 35, 70);
	baseSprite->drawString(highScore, 30, 90);

	baseSprite->drawRect(pointer.x, pointer.y, pointer.width, pointer.height, TFT_GOLD);

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

