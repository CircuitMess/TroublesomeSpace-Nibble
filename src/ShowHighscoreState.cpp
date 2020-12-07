
//
// Created by Domagoj on 13/11/2020.
//

#include "ShowHighscoreState.h"
#include "EraseHighscoreState.h"
#include "Menu.h"
#include "GameState.h"


ShowHighscoreState *ShowHighscoreState::instance = nullptr;

ShowHighscoreState::ShowHighscoreState(){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();

	instance = this;

}

void ShowHighscoreState::enter(Game &_game){

	game = &_game;

	hs.begin();

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

	baseSprite->drawLine(40, 40, 70, 40, TFT_GOLD);
	baseSprite->drawLine(80, 40, 100, 40, TFT_GOLD);

	baseSprite->drawLine(40, 55, 70, 55, TFT_GOLD);
	baseSprite->drawLine(80, 55, 100, 55, TFT_GOLD);

	baseSprite->drawLine(40, 70, 70, 70, TFT_GOLD);
	baseSprite->drawLine(80, 70, 100, 70, TFT_GOLD);

	//baseSprite->drawLine(40, 85, 70, 85, TFT_GOLD);
	//baseSprite->drawLine(80, 85, 100, 85, TFT_GOLD);

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(2);
	baseSprite->setTextColor(TFT_WHITE);
	baseSprite->drawString(highscore, 30, 10);

	baseSprite->setTextFont(1);
	baseSprite->drawNumber(1, 30, 30);
	baseSprite->drawNumber(2, 30, 45);
	baseSprite->drawNumber(3, 30, 60);

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_LIGHTGREY);
	baseSprite->drawString(backMessage, 80, 120);
	baseSprite->drawString(deleteMessage, 0, 120);


	for(int i = 1; i <= 3; i++){

		baseSprite->setCursor(30, 30+(i-1)*15);

		if(i <= hs.dataCount()){
			baseSprite->printf("%.5s    %d", hs.get(i - 1).name, hs.get(i - 1).score);
		}else{
			baseSprite->printf("    /     /  ");
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

