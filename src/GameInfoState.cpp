#include "GameInfoState.h"
#include "Menu.h"
#include "bitmaps/player/invisiblePlayer.hpp"
#include "bitmaps/player/player.hpp"

GameInfoState *GameInfoState::instance = nullptr;

GameInfoState::GameInfoState(){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();

	instance = this;

	baseSprite->clear(TFT_BLACK);
	draw();
	display->commit();
}

GameInfoState::~GameInfoState(){

}

void GameInfoState::enter(Game &_game){

	game = &_game;

	upState = false;
	downState = false;
	bState = false;


	Input::getInstance()->setBtnPressCallback(BTN_UP, buttonUpPressed);
	Input::getInstance()->setBtnPressCallback(BTN_DOWN, buttonDownPressed);
	Input::getInstance()->setBtnPressCallback(BTN_B, buttonBPressed);
	Input::getInstance()->setBtnReleaseCallback(BTN_UP, buttonUpReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_DOWN, buttonDownReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_B, buttonBReleased);
}

void GameInfoState::exit(){

	Input::getInstance()->removeBtnPressCallback(BTN_UP);
	Input::getInstance()->removeBtnPressCallback(BTN_DOWN);
	Input::getInstance()->removeBtnPressCallback(BTN_B);
	Input::getInstance()->removeBtnReleaseCallback(BTN_UP);
	Input::getInstance()->removeBtnReleaseCallback(BTN_DOWN);
	Input::getInstance()->removeBtnReleaseCallback(BTN_B);
}

void GameInfoState::loop(uint){

	baseSprite->clear(TFT_BLACK);
	draw();
	display->commit();

	states();
}

void GameInfoState::draw(){

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_GOLD);
	baseSprite->drawString("X", 5, 3);

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

	baseSprite->drawIcon(invPlayer, 68, 75, 12, 18, 1, TFT_BLACK);
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
	baseSprite->drawString("Fuel", 95, 106);
	baseSprite->drawString("Player:", 3, 40);
	baseSprite->drawString("Invisible:", 3, 79);
	baseSprite->drawString("Btn (A)", 92, 79);

}

void GameInfoState::states(){

	if(bState){
		game->changeState(new Menu());
	}

	else if(upState){
		scrollFactor--;
		if(scrollFactor <= 0){
			scrollFactor = 0;
		}
	}

	else if(downState){
		scrollFactor++;
		if(scrollFactor >= 255){
			scrollFactor = 255;
		}
	}


	if(millis() - previousArrowTime > 200){
		step++;
		if(step > 2)
			step = 0;
		previousArrowTime = millis();
	}
}

void GameInfoState::buttonUpPressed(){
	instance->upState = true;
}
void GameInfoState::buttonDownPressed(){
	instance->downState = true;
}
void GameInfoState::buttonBPressed(){
	instance->bState = true;
}
void GameInfoState::buttonUpReleased(){
	instance->upState = false;
}
void GameInfoState::buttonDownReleased(){
	instance->downState = false;
}
void GameInfoState::buttonBReleased(){
	instance->bState =false;
}
