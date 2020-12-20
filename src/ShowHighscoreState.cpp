
//
// Created by Domagoj on 13/11/2020.
//

#include "ShowHighscoreState.h"
#include "EraseHighscoreState.h"
#include "GameState.h"
#include "Highscore/Highscore.h"
#include "bitmaps/states/highscore.hpp"


ShowHighscoreState *ShowHighscoreState::instance = nullptr;

ShowHighscoreState::ShowHighscoreState(){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();

	instance = this;

}

void ShowHighscoreState::enter(Game &_game){

	game = &_game;

	aState = false;
	bState = false;

	baseSprite->clear(TFT_BLACK);

	Input::getInstance()->setBtnPressCallback(BTN_A, buttonAPressed);
	Input::getInstance()->setBtnPressCallback(BTN_B, buttonBPressed);

	Input::getInstance()->setBtnReleaseCallback(BTN_A, buttonAReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_B, buttonBReleased);

}

void ShowHighscoreState::exit(){


	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_B);

	Input::getInstance()->removeBtnReleaseCallback(BTN_A);
	Input::getInstance()->removeBtnReleaseCallback(BTN_B);

}

void ShowHighscoreState::loop(uint){

	draw();

	display->commit();

	baseSprite->clear(TFT_BLACK);

	if(aState)
		game->changeState(new EraseHighscoreState());
	if(bState)
		game->changeState(new Menu());

}

void ShowHighscoreState::draw(){

	baseSprite->drawIcon(highscoreScreen,0,0,128,128);

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_LIGHTGREY);
	baseSprite->drawString(backMessage, 80, 120);
	baseSprite->drawString(deleteMessage, 0, 120);


	for(int i = 0; i < 5; i++){

		baseSprite->setCursor(30, 37+i*16);

		if((i+1) <= Highscore.dataCount()){
			baseSprite->printf(" %s   %d", Highscore.get(i).name, Highscore.get(i).score);
		}else{
			baseSprite->printf("   -     -  ");
		}
	}

}

void ShowHighscoreState::buttonAPressed(){

	instance->aState = true;
}

void ShowHighscoreState::buttonBPressed(){

	instance->bState = true;
}

void ShowHighscoreState::buttonAReleased(){

	instance->aState = false;
}

void ShowHighscoreState::buttonBReleased(){

	instance->bState = false;
}

