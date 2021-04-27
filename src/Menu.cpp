#include "Menu.h"
#include "GameState.h"
#include "ShowHighscoreState.h"
#include "EraseHighscoreState.h"
#include "GameInfoState.h"
#include "bitmaps/states/homescreen.hpp"
#include "Melody/Melody.h"

TroublesomeSpace::Menu *TroublesomeSpace::Menu::instance = nullptr;

TroublesomeSpace::Menu::Menu(Display &_display){

	display = &_display;
	baseSprite = display->getBaseSprite();

	instance = this;

	baseSprite->clear(TFT_BLACK);
	draw();
	display->commit();
}

void TroublesomeSpace::Menu::enter(Game &_game){

	game = &_game;

	melodyTime = Melody.playMelody(MENU, true);

	menuOrder = 1;

	Input::getInstance()->setBtnPressCallback(BTN_UP, buttonUpPressed);
	Input::getInstance()->setBtnPressCallback(BTN_DOWN, buttonDownPressed);
	Input::getInstance()->setBtnPressCallback(BTN_A, buttonAPressed);
	Input::getInstance()->setBtnReleaseCallback(BTN_UP, buttonUpReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_DOWN, buttonDownReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_A, buttonAReleased);

}

void TroublesomeSpace::Menu::exit(){

	Melody.playMelody(STOP,false);

	Input::getInstance()->removeBtnPressCallback(BTN_UP);
	Input::getInstance()->removeBtnPressCallback(BTN_DOWN);
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnReleaseCallback(BTN_UP);
	Input::getInstance()->removeBtnReleaseCallback(BTN_DOWN);
	Input::getInstance()->removeBtnReleaseCallback(BTN_A);
}

void TroublesomeSpace::Menu::loop(uint t){

	baseSprite->clear(TFT_BLACK);
	draw();
	display->commit();

	states();
}

void TroublesomeSpace::Menu::states(){

	if(instance->upState){
		menuOrder--;
		if(menuOrder < 1){
			menuOrder = 1;
		}

		step = 0;
		previousArrowTime = millis();
		instance->upState = false;
	}
	if(instance->downState){
		menuOrder++;
		if(menuOrder > 5){
			menuOrder = 5;
		}

		step = 0;
		previousArrowTime = millis();
		instance->downState = false;
	}

	if(millis() - previousArrowTime > 200){
		step++;
		if(step > 2)
			step = 0;
		previousArrowTime = millis();
	}

	msgPos--;
	if(msgPos <= -40*5){
		msgPos = 128;
	}

	if(aState && menuOrder == 1){
		game->changeState(new TroublesomeSpace::GameState(*display));
	}
	if(aState && menuOrder == 2){
		Melody.playMelody(STOP,false);
		game->changeState(new TroublesomeSpace::ShowHighscoreState(*display));
	}
	if(aState && menuOrder == 3){
		game->changeState(new TroublesomeSpace::EraseHighscoreState(*display));
	}
	if(aState && menuOrder == 4){
		game->changeState(new TroublesomeSpace::GameInfoState(*display));
	}
	if(aState && menuOrder == 5){

	}
}

void TroublesomeSpace::Menu::draw(){

	baseSprite->drawIcon(homescreen, 0, 0, 128, 128);

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_WHITE);

	if(menuOrder == 1){
		baseSprite->drawString(newGame, 40, 105);
	}
	if(menuOrder == 2){
		baseSprite->drawString(highScore, 35, 105);
	}
	if(menuOrder == 3){
		baseSprite->drawString(eraseHighscore, 20, 105);
	}
	if(menuOrder == 4){
		baseSprite->drawString(info, 52, 105);
	}
	if(menuOrder == 5){
		baseSprite->drawString(quit, 52, 105);
	}

	baseSprite->setTextColor(TFT_DARKGREY);
	baseSprite->setTextFont(1);
	baseSprite->setTextSize(1);
	baseSprite->drawString(infoMessage,msgPos,1);

	drawTriangleArrows();
}

void TroublesomeSpace::Menu::drawTriangleArrows(){

	if(menuOrder <= 1){
		baseSprite->fillTriangle(64, 120 + step, 59, 115 + step, 69, 115 + step, TFT_WHITE);
	}
	else if(menuOrder >= 5){
		baseSprite->fillTriangle(64, 95 - step, 59, 101 - step, 69, 101 - step, TFT_WHITE);
	}
	else{
		baseSprite->fillTriangle(64, 120 + step, 59, 115 + step, 69, 115 + step, TFT_WHITE);
		baseSprite->fillTriangle(64, 95 - step, 59, 101 - step, 69, 101 - step, TFT_WHITE);
	}
}

void TroublesomeSpace::Menu::buttonUpPressed(){
	instance->upState = true;
}

void TroublesomeSpace::Menu::buttonDownPressed(){
	instance->downState = true;
}

void TroublesomeSpace::Menu::buttonAPressed(){
	instance->aState = true;
}

void TroublesomeSpace::Menu::buttonUpReleased(){
	instance->upState = false;
}

void TroublesomeSpace::Menu::buttonDownReleased(){
	instance->downState = false;
}

void TroublesomeSpace::Menu::buttonAReleased(){
	instance->aState = false;
}

