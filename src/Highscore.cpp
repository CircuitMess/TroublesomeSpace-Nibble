//
// Created by Domagoj on 13/11/2020.
//

#include "Highscore.h"
#include "GameOverState.h"
#include <FS.h>
#include <LittleFS.h>

Highscore *Highscore::instance = nullptr;

Highscore::Highscore(int _score, bool _previousState, Melody *mel){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();

	instance = this;

	melody = mel;

	score = _score;
	previousState = _previousState;


}

void Highscore::enter(Game &_game){

	game = &_game;

	upState = false;
	downState = false;
	aState = false;
	bState = false;

	Input::getInstance()->setBtnPressCallback(BTN_UP, buttonUpPressed);
	Input::getInstance()->setBtnPressCallback(BTN_DOWN, buttonDownPressed);
	Input::getInstance()->setBtnPressCallback(BTN_A, buttonAPressed);
	Input::getInstance()->setBtnPressCallback(BTN_B, buttonBPressed);

	Input::getInstance()->setBtnReleaseCallback(BTN_UP, buttonUpReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_DOWN, buttonDownReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_A, buttonAReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_B, buttonBReleased);

}

void Highscore::exit(){

	score = 0;

	Input::getInstance()->removeBtnPressCallback(BTN_UP);
	Input::getInstance()->removeBtnPressCallback(BTN_DOWN);
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_B);

	Input::getInstance()->removeBtnReleaseCallback(BTN_UP);
	Input::getInstance()->removeBtnReleaseCallback(BTN_DOWN);
	Input::getInstance()->removeBtnReleaseCallback(BTN_A);
	Input::getInstance()->removeBtnReleaseCallback(BTN_B);

}

void Highscore::loop(uint){

	if(previousState){                // Game over -> Highscore

		baseSprite->clear(TFT_BLACK);

		draw();


		display->commit();
	}else{                            // Menu -> Highscore

		baseSprite->clear(TFT_BLACK);

		draw();

		display->commit();
	}

	states();
}

void Highscore::states(){

	if(upState){

	}else if(downState){

	}else if(aState){

	}else if(bState){
		if(previousState){

			baseSprite->clear(TFT_BLACK);
			game->changeState(new GameOverState(L, melody, score, true));
			display->commit();

		}else{

			baseSprite->clear(TFT_BLACK);
			game->changeState(new Menu(melody));
			display->commit();
		}
	}
}

void Highscore::draw(){

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(2);
	baseSprite->setTextColor(TFT_WHITE);
	baseSprite->drawString(highscore, 30, 10);

	baseSprite->setTextFont(1);
	baseSprite->drawNumber(1, 30, 35);
	baseSprite->drawNumber(2, 30, 50);
	baseSprite->drawNumber(3, 30, 65);
	baseSprite->drawNumber(4, 30, 80);
	baseSprite->drawNumber(5, 30, 95);

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_LIGHTGREY);
	baseSprite->drawString(confirfmMessage, 1, 120);

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_LIGHTGREY);
	baseSprite->drawString(backMessage, 80, 120);
}

void Highscore::begin(){

}

void Highscore::add(Score &_score){

}

void Highscore::clear(){

}

void Highscore::save(){

}

void Highscore::load(){

}

void Highscore::buttonUpPressed(){

	instance->upState = true;
}

void Highscore::buttonDownPressed(){

	instance->downState = true;
}

void Highscore::buttonAPressed(){

	instance->aState = true;
}

void Highscore::buttonBPressed(){

	instance->bState = true;
}

void Highscore::buttonUpReleased(){

	instance->upState = false;
}

void Highscore::buttonDownReleased(){

	instance->downState = false;
}

void Highscore::buttonAReleased(){

	instance->aState = false;
}

void Highscore::buttonBReleased(){

	instance->bState = false;
}

