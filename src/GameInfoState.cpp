#include "GameInfoState.h"
#include "Menu.h"
#include "bitmaps/player/TS_invisiblePlayer.hpp"
#include "bitmaps/player/TS_player.hpp"
#include "bitmaps/objects/TS_fuel.hpp"
#include "bitmaps/objects/TS_ore.hpp"

TroublesomeSpace::GameInfoState *TroublesomeSpace::GameInfoState::instance = nullptr;

TroublesomeSpace::GameInfoState::GameInfoState(Display &_display){

	display = &_display;
	baseSprite = display->getBaseSprite();

	instance = this;

	baseSprite->clear(TFT_BLACK);
	draw();
	display->commit();
}

TroublesomeSpace::GameInfoState::~GameInfoState(){

}

void TroublesomeSpace::GameInfoState::enter(Game &_game){

	game = &_game;

	Input::getInstance()->setBtnPressCallback(BTN_A, buttonAPressed);
	Input::getInstance()->setBtnPressCallback(BTN_B, buttonBPressed);
	Input::getInstance()->setBtnReleaseCallback(BTN_A, buttonAReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_B, buttonBReleased);
}

void TroublesomeSpace::GameInfoState::exit(){

	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_B);
	Input::getInstance()->removeBtnReleaseCallback(BTN_A);
	Input::getInstance()->removeBtnReleaseCallback(BTN_B);
}

void TroublesomeSpace::GameInfoState::loop(uint){

	baseSprite->clear(TFT_BLACK);
	draw();
	display->commit();

	states();
}

void TroublesomeSpace::GameInfoState::draw(){

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_GOLD);
	baseSprite->drawString("X", 5, 3);
	baseSprite->drawIcon(oreIcon,20,5,3,3,1,TFT_BLACK);

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_GREEN);
	baseSprite->drawString("X", 115, 3);

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_LIGHTGREY);
	baseSprite->drawString("X", 5, 118);

	baseSprite->drawRect(64, 121, 64, 7, TFT_PURPLE);
	baseSprite->fillRect(65, 122, 62, 5, TFT_BLUE);
	baseSprite->drawIcon(fuelIcon,118,106,8,8,1,TFT_BLACK);

	if(invisible){
		baseSprite->drawIcon(invPlayer, 68, 75, 12, 18, 1, TFT_BLACK);
	}
	else{
		baseSprite->drawIcon(playerShip, 68, 75, 12, 18, 1, TFT_BLACK);
	}
	baseSprite->drawIcon(playerShip, 58, 35, 12, 18, 1, TFT_BLACK);
	baseSprite->fillTriangle(50-step, 44, 55-step, 41, 55-step, 47, TFT_LIGHTGREY);
	baseSprite->fillTriangle(78+step, 44, 73+step, 41, 73+step, 47, TFT_LIGHTGREY);
	baseSprite->fillTriangle(64, 27-step, 61, 32-step, 67, 32-step, TFT_LIGHTGREY);
	baseSprite->fillTriangle(64, 61+step, 61, 56+step, 67, 56+step, TFT_LIGHTGREY);

	baseSprite->fillTriangle(59-step, 5, 64-step, 2, 64-step, 8, TFT_LIGHTGREY);
	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_LIGHTGREY);
	baseSprite->drawString("B", 67, 2);

	baseSprite->setTextColor(TFT_RED);
	baseSprite->drawString("Score", 5, 15);
	baseSprite->drawString("Lives", 92, 15);
	baseSprite->drawString("InvCounter", 3, 106);
	baseSprite->drawString("Fuel", 90, 106);
	baseSprite->drawString("Player:", 3, 40);
	baseSprite->drawString("Invisible:", 3, 79);

	baseSprite->setTextColor(TFT_GOLD);
	baseSprite->drawString("(A)", 92, 79);

}

void TroublesomeSpace::GameInfoState::states(){

	if(aState){
		invisible = !invisible;
		aState = false;
	}

	if(bState){
		game->changeState(new Menu(*display));
	}


	if(millis() - previousArrowTime > 200){
		step++;
		if(step > 2)
			step = 0;
		previousArrowTime = millis();
	}
}

void TroublesomeSpace::GameInfoState::buttonBPressed(){
	instance->bState = true;
}
void TroublesomeSpace::GameInfoState::buttonAPressed(){
	instance->aState = true;
}
void TroublesomeSpace::GameInfoState::buttonAReleased(){
	instance->aState =false;
}
void TroublesomeSpace::GameInfoState::buttonBReleased(){
	instance->bState =false;
}
