#include "State.h"
#include "EraseHighscoreState.h"
#include "ShowHighscoreState.h"
#include "Highscore/Highscore.h"
#include "bitmaps/states/TS_eraseHighscoreStateScreen.hpp"

TroublesomeSpace::EraseHighscoreState *TroublesomeSpace::EraseHighscoreState::instance = nullptr;

TroublesomeSpace::EraseHighscoreState::EraseHighscoreState(Display &_display){

	display = &_display;
	baseSprite = display->getBaseSprite();

	instance = this;

}

void TroublesomeSpace::EraseHighscoreState::enter(Game &_game){

	game = &_game;

	aState = false;
	bState = false;

	Input::getInstance()->setBtnPressCallback(BTN_A, buttonAPressed);
	Input::getInstance()->setBtnPressCallback(BTN_B, buttonBPressed);

	Input::getInstance()->setBtnReleaseCallback(BTN_A, buttonAReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_B, buttonBReleased);

}

void TroublesomeSpace::EraseHighscoreState::exit(){

	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_B);

	Input::getInstance()->removeBtnReleaseCallback(BTN_A);
	Input::getInstance()->removeBtnReleaseCallback(BTN_B);

}

void TroublesomeSpace::EraseHighscoreState::loop(uint){

	if(aState){
		Highscore.clearData();
		game->changeState(new ShowHighscoreState(*display));
	}
	if(bState)
		game->changeState(new ShowHighscoreState(*display));

	baseSprite->clear(TFT_BLACK);

	draw();

	display->commit();

}

void TroublesomeSpace::EraseHighscoreState::draw(){

	baseSprite->drawIcon(eraseHighscoreStateScreen, 0, 0, 128, 128);

	baseSprite->setTextSize(2);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_GOLD);

	baseSprite->setCursor(20,40);
	baseSprite->printCenter(confirm);

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(2);
	baseSprite->setTextColor(TFT_WHITE);

	baseSprite->setCursor(40,60);
	baseSprite->printCenter(yes);

	baseSprite->setCursor(40,80);
	baseSprite->printCenter(no);

}

void TroublesomeSpace::EraseHighscoreState::buttonAPressed(){
	instance->aState = true;
}

void TroublesomeSpace::EraseHighscoreState::buttonBPressed(){
	instance->bState = true;
}

void TroublesomeSpace::EraseHighscoreState::buttonAReleased(){
	instance->aState = false;
}

void TroublesomeSpace::EraseHighscoreState::buttonBReleased(){
	instance->bState = false;
}