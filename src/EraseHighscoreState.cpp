//
// Created by Domagoj on 02/12/2020.
//


#include "State.h"
#include "EraseHighscoreState.h"
#include "ShowHighscoreState.h"
#include "Highscore/Highscore.h"

EraseHighscoreState *EraseHighscoreState::instance = nullptr;

EraseHighscoreState::EraseHighscoreState(){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();

	instance = this;

}

void EraseHighscoreState::enter(Game &_game){

	game = &_game;

	aState = false;
	bState = false;

	Input::getInstance()->setBtnPressCallback(BTN_A, buttonAPressed);
	Input::getInstance()->setBtnPressCallback(BTN_B, buttonBPressed);

	Input::getInstance()->setBtnReleaseCallback(BTN_A, buttonAReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_B, buttonBReleased);

}

void EraseHighscoreState::exit(){

	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_B);

	Input::getInstance()->removeBtnReleaseCallback(BTN_A);
	Input::getInstance()->removeBtnReleaseCallback(BTN_B);

}

void EraseHighscoreState::loop(uint){

	if(aState){
		Highscore.clearData();
		game->changeState(new ShowHighscoreState());
	}
	if(bState)
		game->changeState(new ShowHighscoreState());

	baseSprite->clear(TFT_BLACK);

	draw();

	display->commit();

}

void EraseHighscoreState::draw(){

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(2);
	baseSprite->setTextColor(TFT_GOLD);
	baseSprite->drawString(check, 20, 20);

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(2);
	baseSprite->setTextColor(TFT_WHITE);
	baseSprite->drawString(yes, 40, 70);
	baseSprite->drawString(no, 40, 90);

}

void EraseHighscoreState::buttonAPressed(){
	instance->aState = true;
}

void EraseHighscoreState::buttonBPressed(){
	instance->bState = true;
}

void EraseHighscoreState::buttonAReleased(){
	instance->aState = false;
}

void EraseHighscoreState::buttonBReleased(){
	instance->bState = false;
}